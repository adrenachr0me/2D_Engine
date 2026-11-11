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
    if (player) {
        player->Update(dt,tilemap);
        camera.Follow(player->GetX(),player->GetY());
    }

    camera.ClampToBounds(static_cast<float>(tilemap.GetWorldWidth()), static_cast<float>(tilemap.GetWorldHeight()));
}

void Engine::Render()
{
    ClearScreen(30,30,30);

    SDL_Color red ={255,0,0,255};

    SDL_Color green ={0,255,0,255};


    float screenDotX = camera.WorldToScreenX(400.0f);
    float screenDotY = camera.WorldToScreenY(300.0f);
    primitiveRenderer->DrawCircle(screenDotX, screenDotY, 15, red);
    tilemap.Draw(*primitiveRenderer, camera);
    if (player)
    {
        player->Draw(*primitiveRenderer, camera);
    }



    SDL_RenderPresent(renderer);
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
