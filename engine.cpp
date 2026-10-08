#include "engine.h"
#include <iostream>

Engine::Engine()
{
    window = nullptr;
    renderer = nullptr;
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

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL Error: "
                  << SDL_GetError()
                  << std::endl;

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
        std::cerr << "Window Error: "
                  << SDL_GetError()
                  << std::endl;

        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        nullptr);

    if (renderer == nullptr)
    {
        std::cerr << "Renderer Error: "
                  << SDL_GetError()
                  << std::endl;

        return false;
    }

    running = true;

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

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            std::cout << "Klawisz nacisniety\n";
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            std::cout << "Klikniecie myszy\n";
        }
    }
}

void Engine::Update()
{
}

void Engine::Render()
{
    ClearScreen(30, 30, 30);
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
    Uint32 frameDelay = 1000 / fps;

    while (running)
    {
        Uint32 frameStart =
            SDL_GetTicks();

        ProcessEvents();

        Update();

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
}