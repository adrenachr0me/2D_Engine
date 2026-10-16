//
// Created by telep on 10/10/2026.
//

#include "PrimitiveRenderer.h"
#include<math.h>
#include <algorithm>

PrimitiveRenderer::PrimitiveRenderer(SDL_Renderer* r)
{
    renderer = r;
}

void PrimitiveRenderer::DrawLineSDL(
    float x1,
    float y1,
    float x2,
    float y2,
    SDL_Color color)
{
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a);

    SDL_RenderLine(
        renderer,
        x1,
        y1,
        x2,
        y2);
}

void PrimitiveRenderer::DrawLineIncremental(
    float x1,
    float y1,
    float x2,
    float y2,
    SDL_Color color)
{
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a);

    float dx = x2 - x1;
    float dy = y2 - y1;

    if (dx == 0.0f)
    {
        if (y1 > y2)
            std::swap(y1, y2);

        for (float y = y1; y <= y2; y += 1.0f)
        {
            SDL_RenderPoint(
                renderer,
                x1,
                y);
        }

        return;
    }

    float m = dy / dx;

    if (std::abs(m) <= 1.0f)
    {
        if (x1 > x2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        float y = y1;

        for (float x = x1; x <= x2; x += 1.0f)
        {
            SDL_RenderPoint(
                renderer,
                std::round(x),
                std::round(y));

            y += m;
        }
    }
    else
    {
        if (y1 > y2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        float mInv = dx / dy;
        float x = x1;

        for (float y = y1; y <= y2; y += 1.0f)
        {
            SDL_RenderPoint(
                renderer,
                std::round(x),
                std::round(y));

            x += mInv;
        }
    }
}

void PrimitiveRenderer::DrawPoint(
    float x,
    float y,
    SDL_Color color)
{
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a);

    SDL_RenderPoint(
        renderer,
        x,
        y);
}