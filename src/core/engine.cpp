#include "engine.h"
#include "LineSegment.h"
#include <iostream>

Engine::Engine()
{
    window = nullptr;
    renderer = nullptr;
    primitiveRenderer = nullptr;
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

}

void Engine::Render()
{
    ClearScreen(30,30,30);

    SDL_Color red ={255,0,0,255};

    SDL_Color green ={0,255,0,255};

    Point2D point(
        150.0f,
        250.0f);

    point.Draw(
        *primitiveRenderer,
        green);

    LineSegment line1(
    Point2D(400,300),
    Point2D(400,0));

    line1.Draw(
        *primitiveRenderer,
        red,
        false);

    LineSegment line2(
    Point2D(100,200),
    Point2D(500,200));

    line2.Draw(
        *primitiveRenderer,
        green,
        true);

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