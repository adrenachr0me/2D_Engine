#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <functional>
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
    bool sdlInitialized = false;
    bool failed = false;

    int width;
    int height;
    int fps;

    bool fullscreen;

    Input input;
    Timer timer;
    SDL_Color clearColor{30, 30, 30, 255};

public:

    Engine();
    ~Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    bool Initialize(
        const std::string& title,
        int width,
        int height,
        bool fullscreen,
        int fps,
        bool keyboardEnabled = true,
        bool mouseEnabled = true);

    bool Run(const std::function<void(float)>& onUpdate = {});
    void RequestQuit();
    const Input& GetInput() const;
    float GetFPS() const;
    void SetClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    void ProcessEvents();
    void Update(float dt);
    void Render();
    bool ClearScreen(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a = 255);
    void Shutdown();
};
