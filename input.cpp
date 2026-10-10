#include "Input.h"

void Input::HandleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_EVENT_KEY_DOWN:
            if (event.key.scancode < SDL_SCANCODE_COUNT)
                keys[event.key.scancode] = true;
            break;

        case SDL_EVENT_KEY_UP:
            if (event.key.scancode < SDL_SCANCODE_COUNT)
                keys[event.key.scancode] = false;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button < 8)
                mouseButtons[event.button.button] = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button < 8)
                mouseButtons[event.button.button] = false;
            break;

        case SDL_EVENT_MOUSE_MOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            wheelY = event.wheel.y;
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
    if (scancode < SDL_SCANCODE_COUNT)
        return keys[scancode];
    return false;
}

bool Input::IsMouseButtonDown(Uint8 button) const
{
    if (button < 8)
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