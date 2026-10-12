#include "engine.h"
#include <algorithm>
#include <charconv>
#include <iostream>
#include <string_view>

int main(int argc, char* argv[])
{
    int width = 1280, height = 720, fps = 60;
    bool fullscreen = false, keyboard = true, mouse = true;
    for (int i = 1; i < argc; ++i)
    {
        const std::string_view option = argv[i];
        if (option == "--help")
        {
            std::cout << "Engine2D [--width N] [--height N] [--fps N] "
                         "[--fullscreen] [--no-keyboard] [--no-mouse]\n"
                         "Controls: Escape = quit, R/G/B = color, left/right mouse = color, "
                         "wheel = brightness.\n";
            return 0;
        }
        if (option == "--fullscreen") fullscreen = true;
        else if (option == "--no-keyboard") keyboard = false;
        else if (option == "--no-mouse") mouse = false;
        else if (option == "--width" || option == "--height" || option == "--fps")
        {
            if (++i >= argc)
            {
                std::cerr << "Missing value for " << option << '\n';
                return 1;
            }
            const std::string_view value = argv[i];
            int number = 0;
            const auto result = std::from_chars(value.data(), value.data() + value.size(), number);
            if (result.ec != std::errc{} || result.ptr != value.data() + value.size() || number <= 0)
            {
                std::cerr << "Expected a positive integer for " << option << '\n';
                return 1;
            }
            if (option == "--width") width = number;
            else if (option == "--height") height = number;
            else fps = number;
        }
        else
        {
            std::cerr << "Unknown option: " << option << " (use --help)\n";
            return 1;
        }
    }
    Engine engine;
    if (!engine.Initialize("2D Engine | R/G/B, mouse buttons, wheel | Esc: quit",
                           width, height, fullscreen, fps, keyboard, mouse))
        return 1;
    float brightness = 160.0f;
    const bool success = engine.Run([&](float dt)
    {
        (void)dt; // Available for movement independent of frame rate.
        const Input& input = engine.GetInput();
        if (input.IsKeyDown(SDL_SCANCODE_ESCAPE))
        {
            engine.RequestQuit();
            return;
        }
        brightness = std::clamp(brightness + input.GetMouseWheel() * 15.0f, 30.0f, 255.0f);
        const Uint8 channel = static_cast<Uint8>(brightness);
        Uint8 r = 30, g = 30, b = 30;
        if (input.IsKeyDown(SDL_SCANCODE_R)) r = channel;
        if (input.IsKeyDown(SDL_SCANCODE_G)) g = channel;
        if (input.IsKeyDown(SDL_SCANCODE_B)) b = channel;
        if (input.IsMouseButtonDown(SDL_BUTTON_LEFT)) r = g = channel;
        if (input.IsMouseButtonDown(SDL_BUTTON_RIGHT)) g = b = channel;
        engine.SetClearColor(r, g, b);
    });
    return success ? 0 : 1;
}
