

#ifndef ENGINE2D_POINT2D_H
#define ENGINE2D_POINT2D_H

#pragma once

#include <SDL3/SDL.h>
class PrimitiveRenderer;

class Point2D
{
private:
    float x;
    float y;

public:
    Point2D();
    Point2D(float x, float y);

    float GetX() const;
    float GetY() const;

    void SetX(float x);
    void SetY(float y);
    void SetPosition(float x, float y);

    void Draw(
        PrimitiveRenderer& renderer,
        SDL_Color color) const;
};
#endif //ENGINE2D_POINT2D_H