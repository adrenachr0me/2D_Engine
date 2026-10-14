
#ifndef ENGINE2D_TIMER_H
#define ENGINE2D_TIMER_H
#pragma once
#include <cstdint>

class Timer {
public:
    Timer();
    void Tick();
    float GetDT() const;
    float GetFPS() const;

private:
    uint64_t lastTime = 0;
    float deltaTime = 0.0f;
    float fps = 0.0f;
};
#endif //ENGINE2D_TIMER_H