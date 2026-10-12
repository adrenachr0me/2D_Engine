#include "Timer.h"
#include <SDL3/SDL.h>

Timer::Timer()
{
    lastTime = SDL_GetPerformanceCounter();
    deltaTime = 0.0f;
    fps = 0.0f;
}

void Timer::Tick() {
    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    deltaTime = static_cast<float>(currentTime - lastTime) / static_cast<float>(frequency);

    lastTime = currentTime;

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    if (deltaTime > 0.0f)
    {
        fps = 1.0f / deltaTime;
    }
}

float Timer::GetDT() const{
    return deltaTime;
}

float Timer::GetFPS() const {
    return fps;
}