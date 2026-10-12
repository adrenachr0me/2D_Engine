#include "timer.h"
#include <SDL3/SDL.h>
#include <algorithm>

Timer::Timer()
{
    Reset();
}

void Timer::Reset()
{
    lastTime = SDL_GetPerformanceCounter();
    deltaTime = 0.0f;
    fps = 0.0f;
}

void Timer::Tick() {
    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    const float elapsed = static_cast<float>(currentTime - lastTime) / static_cast<float>(frequency);

    lastTime = currentTime;

    deltaTime = std::min(elapsed, 0.05f);
    fps = elapsed > 0.0f ? 1.0f / elapsed : 0.0f;
}

float Timer::GetDT() const{
    return deltaTime;
}

float Timer::GetFPS() const {
    return fps;
}
