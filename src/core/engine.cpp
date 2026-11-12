#include "engine.h"
#include "LineSegment.h"
#include <iostream>

Engine::Engine()
{
    window = nullptr;
    renderer = nullptr;
    primitiveRenderer = nullptr;
    player = nullptr;
    running = false;

}

Engine::~Engine()
{
Shutdown();
}

bool Engine::Initialize(
    const std::string& title,
    int w,
    int h,
    bool fs,
    int targetFPS)
{
    width = w;
    height = h;
    fullscreen = fs;
    fps = targetFPS;
    Log::Init("logs.txt");
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        ErrorHandle::SDLError("Failed to initialize SDL");
        Shutdown();
        return false;
    }

    Uint32 flags = SDL_WINDOW_RESIZABLE;

    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(
        title.c_str(),
        width,
        height,
        flags);

    if (window == nullptr)
    {
        ErrorHandle::SDLError("Failed to create window");
        Shutdown();
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        nullptr);

    if (renderer == nullptr)
    {
        ErrorHandle::SDLError("Failed to create renderer");
        Shutdown();
        return false;
    }
    primitiveRenderer = new PrimitiveRenderer(renderer);
    const char* basePath = SDL_GetBasePath();
    const std::string assetsPath = std::string(basePath ? basePath : "") + "assets/";
    if (!mapTileset.load_from_file(assetsPath + "tileset.bmp"))
    {
        Log::Error("Failed to load assets/tileset.bmp");
        Shutdown();
        return false;
    }
    if (!tilemap.LoadFromFile(assetsPath + "world.map", &mapTileset))
    {
        Log::Error("Failed to load assets/world.map");
        Shutdown();
        return false;
    }
    if (!victoryMap.LoadFromFile(assetsPath + "victory.map", &mapTileset))
    {
        Log::Error("Failed to load assets/victory.map");
        Shutdown();
        return false;
    }


    const float startX = 624.0f;
    const float startY = 1584.0f;
    player = new Player(startX, startY, input);
    if (!player->HasSprites())
    {
        Log::Error("Failed to load assets/player.bmp");
        Shutdown();
        return false;
    }
    const AABB spawn = player->GetCollider();
    stars.clear();
    starsCollected = 0;

    const int tSize = tilemap.GetTileSize();
    std::vector<std::pair<int, int>> spawnTiles = {
        {15, 12},
        {15, 58},
        {105, 55}
    };

    for (auto [tx, ty] : spawnTiles) {
        while (tilemap.IsSolidAtTile(tx, ty) && tx < tilemap.GetMapWidth() - 3) {
            tx++;
        }

        float worldX = tx * tSize + tSize / 2.0f;
        float worldY = ty * tSize + tSize / 2.0f;
        stars.push_back({worldX, worldY, 12.0f, false});
    }
    currentState = GameState::Playing;

    portal.centerX = 98.0f * tSize;
    portal.centerY = 19.0f * tSize;

    const int baseLeftX = 95;
    const int baseTopY  = 18;
    const int baseCols  = 6;
    const int baseRows  = 3;
    const float padding = 10.0f;

    portal.triggerBox = AABB(
        baseLeftX * tSize - padding,
        baseTopY  * tSize - padding,
        baseCols  * tSize + padding * 2.0f,
        baseRows  * tSize + padding * 2.0f
    );
    portal.active = false;
    if (tilemap.CheckCollision(spawn.x, spawn.y, spawn.w, spawn.h))
    {
        Log::Error("Player start position is blocked");
        Shutdown();
        return false;
    }
    camera = Camera(width, height);
    camera.Follow(startX, startY);
    camera.ClampToBounds(static_cast<float>(tilemap.GetWorldWidth()), static_cast<float>(tilemap.GetWorldHeight()));
    running = true;
    Log::Info("Engine initialized successfully");
    return true;
}

void Engine::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }

        input.HandleEvent(event);
    }
}



void Engine::Update(float dt)
{

    if (currentState == GameState::Victory) {
        if (input.IsKeyDown(SDL_SCANCODE_ESCAPE)) running = false;

        if (player) {
            player->Update(dt, victoryMap);
            camera.Follow(player->GetX(), player->GetY());
        }
        camera.ClampToBounds(static_cast<float>(victoryMap.GetWorldWidth()),
                             static_cast<float>(victoryMap.GetWorldHeight()));
        return;
    }

    if (player) {
        player->Update(dt, tilemap);
        camera.Follow(player->GetX(), player->GetY());
        AABB playerBox = player->GetCollider();

        for (auto& star : stars) {
            if (!star.collected && playerBox.Intersects(star.GetCollider())) {
                star.collected = true;
                starsCollected++;
                if (starsCollected >= 3) {
                    portal.active = true;
                }
            }
        }

        if (portal.active && playerBox.Intersects(portal.triggerBox)) {
            currentState = GameState::Victory;
            player->SetPosition(victoryStartX, victoryStartY);
            camera.Follow(victoryStartX, victoryStartY);
            camera.ClampToBounds(static_cast<float>(victoryMap.GetWorldWidth()),
                                 static_cast<float>(victoryMap.GetWorldHeight()));
            return;
        }
    }

    camera.ClampToBounds(static_cast<float>(tilemap.GetWorldWidth()),
                         static_cast<float>(tilemap.GetWorldHeight()));
}

void Engine::Render()
{
    if (currentState == GameState::Victory) {
        ClearScreen(15, 20, 30);
        victoryMap.Draw(*primitiveRenderer, camera);
        if (player) player->Draw(*primitiveRenderer, camera);
        DrawVictoryText();
        SDL_RenderPresent(renderer);
        return;
    }


    ClearScreen(30,30,30);
    tilemap.Draw(*primitiveRenderer, camera);

    if (portal.active) {
        float px = portal.centerX - camera.GetX();
        float py = portal.centerY - camera.GetY();

        SDL_Color outerCyan{0, 255, 220, 255};
        SDL_Color innerWhite{200, 255, 255, 255};

        primitiveRenderer->DrawCircle(px, py, 20.0f, outerCyan);
        primitiveRenderer->DrawCircle(px, py, 10.0f, innerWhite);
    }



    for (const auto& star : stars) {
        if (star.collected) continue;

        float sx = star.x - camera.GetX();
        float sy = star.y - camera.GetY();
        float r  = star.radius;
        float diag = r * 0.7f;

        SDL_Color gold{255, 215, 0, 255};
        SDL_Color lightGold{255, 250, 160, 255};

        primitiveRenderer->DrawLineSDL(sx - r, sy, sx + r, sy, gold);
        primitiveRenderer->DrawLineSDL(sx, sy - r, sx, sy + r, gold);

        primitiveRenderer->DrawLineSDL(sx - diag, sy - diag, sx + diag, sy + diag, lightGold);
        primitiveRenderer->DrawLineSDL(sx - diag, sy + diag, sx + diag, sy - diag, lightGold);
    }
    if (player)
    {
        player->Draw(*primitiveRenderer, camera);
    }



    SDL_RenderPresent(renderer);
}

void Engine::DrawVictoryText()
{
    const float vw = static_cast<float>(camera.GetViewportWidth());

    // ciemny panel pod tekstem, zeby bylo go widac na kafelkach
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
    SDL_FRect panel{vw / 2.0f - 450.0f, 70.0f, 900.0f, 180.0f};
    SDL_RenderFillRect(renderer, &panel);

    auto centered = [&](const std::string& text, float y, float scale,
                        Uint8 r, Uint8 g, Uint8 b) {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_SetRenderScale(renderer, scale, scale);
        float x = (vw / scale - text.size() * 8.0f) / 2.0f;   // znak = 8 px
        SDL_RenderDebugText(renderer, x, y / scale, text.c_str());
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    };

    centered("MISSION COMPLETE", 100.0f, 6.0f, 50, 255, 150);
    centered("Stars collected: " + std::to_string(starsCollected) + "/3",
             170.0f, 3.0f, 255, 215, 0);
    centered("Press ESC to exit", 215.0f, 2.0f, 255, 255, 255);
}

void Engine::ClearScreen(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a)
{
    SDL_SetRenderDrawColor(
        renderer,
        r, g, b, a);

    SDL_RenderClear(renderer);
}

void Engine::Run()
{
    if (fps <= 0)
        fps = 60;

    Uint32 frameDelay = 1000 / fps;

    while (running)
    {
        Uint32 frameStart =
            SDL_GetTicks();

        timer.Tick();

        ProcessEvents();

        Update(timer.GetDT());

        Render();
        input.EndFrame();

        Uint32 frameTime =
            SDL_GetTicks() - frameStart;

        if (frameTime < frameDelay)
        {
            SDL_Delay(
                frameDelay - frameTime);
        }
    }
}

void Engine::Shutdown()
{
    if (player) {
        delete player;
        player = nullptr;
    }
    if (primitiveRenderer)
    {
        delete primitiveRenderer;
        primitiveRenderer = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
    Log::Shutdown();
}
