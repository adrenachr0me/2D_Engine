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
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        nullptr);

    if (renderer == nullptr)
    {
        ErrorHandle::SDLError("Failed to create renderer");
        return false;
    }
    primitiveRenderer = new PrimitiveRenderer(renderer);
    player = new Player(tilemap.GetWorldWidth() / 2.0f, tilemap.GetWorldHeight() / 2.0f, input);
    camera = Camera(width,height);
    if (!mapTileset.load_from_file("assets/tileset.bmp"))
    {
        Log::Error("Failed to load tileset.bmp");
    }
    // 1. Задаем размер карты в 2 экрана
    const int mapW = 80;
    const int mapH = 45;
    const int tSize = 32;

    const int TILE_GRASS = 0;
    const int TILE_WALL  = 1090;

    // Заполняем всё поле травой
    std::vector<int> testMap(mapW * mapH, TILE_GRASS);

    // 2. Внешние границы мира (периметр)
    for (int x = 0; x < mapW; ++x) {
        testMap[0 * mapW + x] = TILE_WALL;              // Верхний край
        testMap[(mapH - 1) * mapW + x] = TILE_WALL;     // Нижний край
    }
    for (int y = 0; y < mapH; ++y) {
        testMap[y * mapW + 0] = TILE_WALL;              // Левый край
        testMap[y * mapW + (mapW - 1)] = TILE_WALL;     // Правый край
    }

    // 3. Строим внутреннюю комнату 1 (слева, 15x15 тайлов с дверным проемом)
    for (int x = 10; x <= 25; ++x) {
        testMap[10 * mapW + x] = TILE_WALL;
        testMap[25 * mapW + x] = TILE_WALL;
    }
    for (int y = 10; y <= 25; ++y) {
        testMap[y * mapW + 10] = TILE_WALL;
        if (y < 16 || y > 19) { // Оставляем проем (дверь) шириной 4 тайла
            testMap[y * mapW + 25] = TILE_WALL;
        }
    }

    // 4. Строим коридор/стену по центру карты
    for (int y = 5; y < 40; ++y) {
        if (y < 20 || y > 24) { // Проход по центру
            testMap[y * mapW + 40] = TILE_WALL;
        }
    }

    // 5. Расставляем каменные колонны 2x2 в правой части карты
    for (int cy = 10; cy <= 30; cy += 10) {
        for (int cx = 50; cx <= 70; cx += 10) {
            testMap[cy * mapW + cx] = TILE_WALL;
            testMap[cy * mapW + (cx + 1)] = TILE_WALL;
            testMap[(cy + 1) * mapW + cx] = TILE_WALL;
            testMap[(cy + 1) * mapW + (cx + 1)] = TILE_WALL;
        }
    }

    // 6. Передаем готовую карту в Tilemap
    tilemap = Tilemap(mapW, mapH, tSize, &mapTileset);
    tilemap.SetTiles(testMap, mapW, mapH);
    player->SetPosition(420,600);
    //if (!player->HasSprites()) {
     //   ErrorHandle::SDLError("Failed to load assets/player.bmp");
       // Shutdown();
        //return false;
    //}
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
