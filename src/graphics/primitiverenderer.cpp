

#include "PrimitiveRenderer.h"
#include<math.h>
#include <algorithm>
#include <cmath>

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

void PrimitiveRenderer::DrawPolilane(const std::vector<Point2D>& points, bool isClosed, SDL_Color color) {
    if (points.size() < 2)
    {
        return;
    }

    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        DrawLineSDL(
            points[i].GetX(),
            points[i].GetY(),
            points[i + 1].GetX(),
            points[i + 1].GetY(),
            color
        );
    }

    if (isClosed)
    {
        DrawLineSDL(
            points.back().GetX(),
            points.back().GetY(),
            points.front().GetX(),
            points.front().GetY(),
            color
        );
    }
};


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
void PrimitiveRenderer::DrawCircle(float x0, float y0, int r, SDL_Color color)
{
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a);
    const float PI = 3.1415926535f;

    for (float alpha = 0.0; alpha <= PI / 2.0; alpha += 0.01)
    {
        float dx = std::round(r * std::cos(alpha));
        float dy = std::round(r * std::sin(alpha));
        DrawPoint(x0 + dx,y0 + dy,color);
        DrawPoint(x0 - dx,y0 + dy,color);
        DrawPoint(x0-dx,y0 - dy,color);
        DrawPoint(x0+dx,y0-dy,color);
    }
}
void PrimitiveRenderer::DrawElipse(float x0, float y0, int  r_x, int r_y, SDL_Color color) {
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a);
    const float PI = 3.1415926535f;

    for (float alpha = 0.0; alpha <= PI / 2.0; alpha += 0.01) {
        float dx = std::round(r_x * std::cos(alpha));
        float dy = std::round(r_y * std::sin(alpha));
        DrawPoint(x0 + dx,y0 + dy,color);
        DrawPoint(x0 - dx,y0 + dy,color);
        DrawPoint(x0-dx,y0 - dy,color);
        DrawPoint(x0+dx,y0-dy,color);
    }
}