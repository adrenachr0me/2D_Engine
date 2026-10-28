

#ifndef ENGINE2D_RENDER_H
#define ENGINE2D_RENDER_H

#include <SDL3/SDL.h>
#include <vector>
#include "point2D.h"
#include "linesegment.h"
#pragma once

class PrimitiveRenderer
{
private:
    SDL_Renderer* renderer;
    SDL_Color GetPixelColor(SDL_Surface* surface, int x, int y);
    void SetPixelColor(SDL_Surface* surface, int x, int y, SDL_Color color);
public:
    PrimitiveRenderer(SDL_Renderer* r);
    bool ColorsMatch(const SDL_Color& c1, const SDL_Color& c2);
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


    void DrawPolilane(const std::vector<Point2D>& points, bool isClosed, SDL_Color color);
    void DrawCircle(float x0, float y0, int r, SDL_Color color);
    void DrawFilledCircle(float x0, float y0, int r, SDL_Color color);
    void DrawElipse(float x0, float y0, int r_x, int r_y, SDL_Color color);
    void DrawFilledEllipse(float x0, float y0, int r_x, int r_y, SDL_Color color);
    void DrawPolygon(const std::vector<Point2D>& points, SDL_Color color);


    void BoundaryFill(Point2D seed, SDL_Color fillColor, SDL_Color boundaryColor);
    void FloodFill(Point2D seed, SDL_Color fillColor, SDL_Color backgroundColor);
};


#endif //ENGINE2D_RENDER_H