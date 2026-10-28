

#ifndef ENGINE2D_POINT2D_H
#define ENGINE2D_POINT2D_H

#pragma once

#include <SDL3/SDL.h>
#include "core/object_hierarchy.h"
class PrimitiveRenderer;

class Point2D:public ShapeObject
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

    void Draw(PrimitiveRenderer& renderer) override;
    void Translate(float tx, float ty) override;
    void Rotate(float alpha) override;
    void Rotate(float alpha, const Point2D& center) override;
    void Scale(float kx, float ky) override;
    void Scale(float kx, float ky, const Point2D& center) override;
};
#endif //ENGINE2D_POINT2D_H