#pragma once

#include <SDL3/SDL.h>
#include <string>

class Engine
{
private:

    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;

    int width;
    int height;
    int fps;

    bool fullscreen;

public:

    Engine();
    ~Engine();

    bool Initialize(
        const std::string& title,
        int width,
        int height,
        bool fullscreen,
        int fps);

    void Run();

    void ProcessEvents();
    void Update();
    void Render();
    void ClearScreen(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a = 255);
    void Shutdown();
};