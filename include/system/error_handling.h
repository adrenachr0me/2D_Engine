//
// Created by telep on 10/10/2026.
//

#ifndef ENGINE2D_ERROR_HANDLING_H
#define ENGINE2D_ERROR_HANDLING_H
#pragma once

#include <iostream>
#include <string_view>
#include <SDL3/SDL.h>
#include "logs.h" // Подключаем логгер

class ErrorHandle
{
public:
    static void Error(std::string_view msg)
    {
        Log::Error(msg);
    }

    static void SDLError(std::string_view action)
    {
        std::string fullMsg = std::string(action) + " -> SDL: " + SDL_GetError();
        Log::Error(fullMsg);
    }
};

#endif //ENGINE2D_ERROR_HANDLING_H