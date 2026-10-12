#include "engine.h"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
void Expect(bool condition, const char* message)
{
    if (!condition)
        throw std::runtime_error(message);
}

SDL_Event Key(SDL_EventType type, SDL_Scancode code)
{
    SDL_Event event{};
    event.type = type;
    event.key.scancode = code;
    return event;
}

SDL_Event Wheel(float amount)
{
    SDL_Event event{};
    event.type = SDL_EVENT_MOUSE_WHEEL;
    event.wheel.y = amount;
    return event;
}

void TestInput()
{
    Input input;
    input.HandleEvent(Key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_R));
    Expect(input.IsKeyDown(SDL_SCANCODE_R), "Key down was not retained");
    input.EndFrame();
    Expect(input.IsKeyDown(SDL_SCANCODE_R), "Held key was lost at frame end");
    input.HandleEvent(Key(SDL_EVENT_KEY_UP, SDL_SCANCODE_R));
    Expect(!input.IsKeyDown(SDL_SCANCODE_R), "Key up was ignored");
    input.HandleEvent(Key(SDL_EVENT_KEY_DOWN, static_cast<SDL_Scancode>(-1)));
    input.HandleEvent(Key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_COUNT));
    Expect(!input.IsKeyDown(static_cast<SDL_Scancode>(-1)), "Negative scancode was accepted");
    Expect(!input.IsKeyDown(SDL_SCANCODE_COUNT), "Out-of-range scancode was accepted");

    SDL_Event button{};
    button.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
    button.button.button = SDL_BUTTON_LEFT;
    input.HandleEvent(button);
    Expect(input.IsMouseButtonDown(SDL_BUTTON_LEFT), "Mouse press was ignored");
    button.type = SDL_EVENT_MOUSE_BUTTON_UP;
    input.HandleEvent(button);
    Expect(!input.IsMouseButtonDown(SDL_BUTTON_LEFT), "Mouse release was ignored");
    Expect(!input.IsMouseButtonDown(0) && !input.IsMouseButtonDown(8), "Invalid mouse button was accepted");

    SDL_Event motion{};
    motion.type = SDL_EVENT_MOUSE_MOTION;
    motion.motion.x = 120.5f;
    motion.motion.y = 40.25f;
    input.HandleEvent(motion);
    float x, y;
    input.GetMousePos(x, y);
    Expect(x == 120.5f && y == 40.25f, "Mouse position was not updated");

    input.HandleEvent(Wheel(1.0f));
    input.HandleEvent(Wheel(2.0f));
    Expect(input.GetMouseWheel() == 3.0f, "Wheel events did not accumulate");
    auto flipped = Wheel(1.0f);
    flipped.wheel.direction = SDL_MOUSEWHEEL_FLIPPED;
    input.HandleEvent(flipped);
    Expect(input.GetMouseWheel() == 2.0f, "Flipped wheel direction was ignored");
    input.EndFrame();
    Expect(input.GetMouseWheel() == 0.0f, "Wheel state leaked into the next frame");

    input.SetEnabled(false, true);
    input.HandleEvent(Key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_R));
    input.HandleEvent(Wheel(1.0f));
    Expect(!input.IsKeyDown(SDL_SCANCODE_R) && input.GetMouseWheel() == 1.0f,
           "Keyboard disabling also disabled mouse input");
    input.SetEnabled(true, false);
    input.HandleEvent(Key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_R));
    input.HandleEvent(Wheel(1.0f));
    input.HandleEvent(motion);
    button.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
    input.HandleEvent(button);
    input.GetMousePos(x, y);
    Expect(input.IsKeyDown(SDL_SCANCODE_R) && input.GetMouseWheel() == 0.0f &&
           !input.IsMouseButtonDown(SDL_BUTTON_LEFT) && x == 0.0f && y == 0.0f,
           "Mouse disabling did not isolate all mouse events");

    SDL_Event focus{};
    focus.type = SDL_EVENT_WINDOW_FOCUS_LOST;
    input.HandleEvent(focus);
    Expect(!input.IsKeyDown(SDL_SCANCODE_R), "Key remained stuck after focus loss");
}

void TestTimer()
{
    Timer timer;
    SDL_Delay(75);
    timer.Tick();
    Expect(std::abs(timer.GetDT() - 0.05f) < 0.001f, "Long frame delta was not capped");
    Expect(timer.GetFPS() > 0.0f && timer.GetFPS() < 20.0f,
           "FPS was incorrectly calculated from capped delta");
    timer.Reset();
    Expect(timer.GetDT() == 0.0f && timer.GetFPS() == 0.0f, "Timer reset retained previous values");
}

void TestLogFilename()
{
    const std::string storage = "test-log.txt-ignored-suffix";
    const std::string_view filename(storage.data(), 12);
    Expect(Log::Init(filename), "Logger rejected a string_view path");
    Log::Error("diagnostic test");
    Log::Shutdown();
    std::ifstream file("test-log.txt");
    const std::string contents((std::istreambuf_iterator<char>(file)), {});
    Expect(contents.find("[ERROR] diagnostic test") != std::string::npos,
           "Diagnostic was not written to the exact requested path");
    file.close();
    std::filesystem::remove("test-log.txt");
}

void TestEngine()
{
    Engine engine;
    Expect(!engine.Run(), "Uninitialized engine was allowed to run");
    Expect(!engine.ClearScreen(0, 0, 0), "Uninitialized renderer was accepted");
    Expect(!engine.Initialize("test", 320, 240, false, 0), "Zero FPS was accepted");
    Expect(!engine.Initialize("test", -1, 240, false, 60), "Invalid resolution was accepted");
    Expect(engine.Initialize("test", 320, 240, false, 60), "Headless initialization failed");
    Expect(!engine.Initialize("test", 320, 240, false, 60), "Repeated initialization was accepted");
    engine.ProcessEvents(); // Drain startup window events before injecting input.
    auto key = Key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_R);
    auto wheel = Wheel(2.0f);
    Expect(SDL_PushEvent(&key) && SDL_PushEvent(&wheel), "Failed to inject input events");
    int frames = 0;
    const Uint64 started = SDL_GetTicksNS();
    Expect(engine.Run([&](float dt)
    {
        Expect(dt >= 0.0f && dt <= 0.05f, "Invalid update delta");
        Expect(engine.GetInput().IsKeyDown(SDL_SCANCODE_R), "Engine did not expose held keyboard state");
        Expect(engine.GetInput().GetMouseWheel() == (frames == 0 ? 2.0f : 0.0f),
               "Wheel event was not scoped to one engine frame");
        engine.SetClearColor(10, 20, 30);
        ++frames;
        if (frames == 5)
            engine.RequestQuit();
    }), "Main loop failed");
    Expect(frames == 5, "RequestQuit did not stop the main loop");
    Expect(SDL_GetTicksNS() - started >= 50000000ULL, "60 FPS limit did not delay frames");
    Expect(engine.GetFPS() > 0.0f, "Engine did not expose timer FPS");
    engine.Shutdown();
    engine.Shutdown();
    Expect(SDL_WasInit(SDL_INIT_VIDEO) == 0, "Video subsystem leaked after shutdown");

    Expect(engine.Initialize("test", 320, 240, false, 60, false, false), "Reinitialization failed");
    key = Key(SDL_EVENT_KEY_DOWN, SDL_SCANCODE_R);
    wheel = Wheel(1.0f);
    Expect(SDL_PushEvent(&key) && SDL_PushEvent(&wheel), "Failed to inject disabled input");
    Expect(engine.Run([&](float)
    {
        Expect(!engine.GetInput().IsKeyDown(SDL_SCANCODE_R) && engine.GetInput().GetMouseWheel() == 0.0f,
               "Engine input switches were ignored");
        SDL_Event quit{};
        quit.type = SDL_EVENT_QUIT;
        Expect(SDL_PushEvent(&quit), "Failed to inject quit event");
    }), "Quit event failed to end the main loop");
    engine.Shutdown();
}
}

int main()
{
    try
    {
        TestInput();
        TestTimer();
        TestLogFilename();
        TestEngine();
        std::cout << "Lab 01 checks passed.\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Test failed: " << error.what() << '\n';
        return 1;
    }
}
