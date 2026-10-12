#include "engine.h"

Engine::Engine()
    : window(nullptr), renderer(nullptr), running(false),
      width(0), height(0), fps(60), fullscreen(false)
{
}

Engine::~Engine()
{
    Shutdown();
}

bool Engine::Initialize(const std::string& title, int w, int h, bool fs,
                        int targetFPS, bool keyboardEnabled, bool mouseEnabled)
{
    if (sdlInitialized)
    {
        Log::Error("Engine is already initialized.");
        return false;
    }
    failed = false;
    if (!Log::Init("logs.txt"))
        return false;
    if (w <= 0 || h <= 0 || targetFPS <= 0)
    {
        Log::Error("Width, height and target FPS must be positive.");
        Shutdown();
        return false;
    }
    width = w;
    height = h;
    fullscreen = fs;
    fps = targetFPS;
    input.SetEnabled(keyboardEnabled, mouseEnabled);
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        ErrorHandle::SDLError("Failed to initialize SDL");
        Shutdown();
        return false;
    }
    sdlInitialized = true;
    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    window = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (!window)
    {
        ErrorHandle::SDLError("Failed to create window");
        Shutdown();
        return false;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        ErrorHandle::SDLError("Failed to create renderer");
        Shutdown();
        return false;
    }
    timer.Reset();
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
            RequestQuit();
        if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
            width = event.window.data1;
            height = event.window.data2;
        }
        input.HandleEvent(event);
    }
}

void Engine::Update(float dt)
{
    // Game-specific logic is supplied to Run via onUpdate.
    (void)dt;
}

void Engine::Render()
{
    if (!ClearScreen(clearColor.r, clearColor.g, clearColor.b, clearColor.a))
        return;
    if (!SDL_RenderPresent(renderer))
    {
        ErrorHandle::SDLError("Failed to present frame");
        failed = true;
        RequestQuit();
    }
}

bool Engine::ClearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (!renderer)
    {
        Log::Error("Cannot clear the screen before initialization.");
        failed = true;
        RequestQuit();
        return false;
    }
    if (!SDL_SetRenderDrawColor(renderer, r, g, b, a) || !SDL_RenderClear(renderer))
    {
        ErrorHandle::SDLError("Failed to clear screen");
        failed = true;
        RequestQuit();
        return false;
    }
    return true;
}

bool Engine::Run(const std::function<void(float)>& onUpdate)
{
    if (!renderer || !running)
    {
        Log::Error("Cannot run an engine that has not been initialized or has stopped.");
        return false;
    }
    const Uint64 frameDuration = 1000000000ULL / static_cast<Uint64>(fps);
    timer.Reset();
    while (running)
    {
        const Uint64 frameStart = SDL_GetTicksNS();
        timer.Tick();
        ProcessEvents();
        if (!running)
            break;
        Update(timer.GetDT());
        if (onUpdate)
            onUpdate(timer.GetDT());
        if (running)
            Render();
        input.EndFrame();
        const Uint64 frameTime = SDL_GetTicksNS() - frameStart;
        if (running && frameTime < frameDuration)
            SDL_DelayNS(frameDuration - frameTime);
    }
    return !failed;
}

void Engine::RequestQuit()
{
    running = false;
}

const Input& Engine::GetInput() const
{
    return input;
}

float Engine::GetFPS() const
{
    return timer.GetFPS();
}

void Engine::SetClearColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    clearColor = SDL_Color{r, g, b, a};
}

void Engine::Shutdown()
{
    running = false;
    input.Reset();
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
    if (sdlInitialized)
    {
        SDL_Quit();
        sdlInitialized = false;
    }
    Log::Shutdown();
}
