//
// Created by telep on 10/10/2026.
//

#ifndef ENGINE2D_RENDER_H
#define ENGINE2D_RENDER_H

#include <SDL3/SDL.h>
#pragma once

class PrimitiveRenderer
{
private:
    SDL_Renderer* renderer;

public:
    PrimitiveRenderer(SDL_Renderer* r);

    void DrawLineSDL(
        float x1,
        float y1,
        float x2,
        float y2,
        SDL_Color color);

    void DrawLineIncremental(
        float x1,
        float y1,
        float x2,
        float y2,
        SDL_Color color);

    void DrawPoint(
    float x,
    float y,
    SDL_Color color);
};

#endif //ENGINE2D_RENDER_H