#include "input.h"
#include <algorithm>
#include <iterator>

void Input::SetEnabled(bool keyboard, bool mouse)
{
    keyboardEnabled = keyboard;
    mouseEnabled = mouse;
    Reset();
}

void Input::Reset()
{
    std::fill(std::begin(keys), std::end(keys), false);
    std::fill(std::begin(mouseButtons), std::end(mouseButtons), false);
    mouseX = mouseY = wheelY = 0.0f;
}

void Input::HandleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            Reset();
            break;

        case SDL_EVENT_KEY_DOWN:
            if (keyboardEnabled && event.key.scancode >= 0 && event.key.scancode < SDL_SCANCODE_COUNT)
                keys[event.key.scancode] = true;
            break;

        case SDL_EVENT_KEY_UP:
            if (keyboardEnabled && event.key.scancode >= 0 && event.key.scancode < SDL_SCANCODE_COUNT)
                keys[event.key.scancode] = false;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (mouseEnabled && event.button.button > 0 && event.button.button < 8)
                mouseButtons[event.button.button] = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (mouseEnabled && event.button.button > 0 && event.button.button < 8)
                mouseButtons[event.button.button] = false;
            break;

        case SDL_EVENT_MOUSE_MOTION:
            if (mouseEnabled)
            {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            }
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            if (mouseEnabled)
                wheelY += event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -event.wheel.y : event.wheel.y;
            break;

        default:
            break;
    }
}

void Input::EndFrame()
{
    wheelY = 0.0f;
}

bool Input::IsKeyDown(SDL_Scancode scancode) const
{
    if (scancode >= 0 && scancode < SDL_SCANCODE_COUNT)
        return keys[scancode];
    return false;
}

bool Input::IsMouseButtonDown(Uint8 button) const
{
    if (button > 0 && button < 8)
        return mouseButtons[button];
    return false;
}

void Input::GetMousePos(float& x, float& y) const
{
    x = mouseX;
    y = mouseY;
}

float Input::GetMouseWheel() const
{
    return wheelY;
}
