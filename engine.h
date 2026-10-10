#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "input.h"
#include "timer.h"
#include "error_handling.h"
#include "logs.h"
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

    Input input;
    Timer timer;
    ErrorHandle error_handle;
    Log log;

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
    void Update(float dt);
    void Render();
    void ClearScreen(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a = 255);
    void Shutdown();
};