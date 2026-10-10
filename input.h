//
// Created by telep on 10/10/2026.
//

#ifndef ENGINE2D_INPUT_H
#define ENGINE2D_INPUT_H
#pragma once

#include <SDL3/SDL.h>

class Input
{
private:
    bool keys[SDL_SCANCODE_COUNT] = {false};
    bool mouseButtons[8] = {false};

    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float wheelY = 0.0f;

public:
    Input() = default;
    void HandleEvent(const SDL_Event& event);
    void EndFrame();

    bool IsKeyDown(SDL_Scancode scancode) const;
    bool IsMouseButtonDown(Uint8 button) const;
    void GetMousePos(float& x, float& y) const;
    float GetMouseWheel() const;
};
#endif //ENGINE2D_INPUT_H