

#include "Point2D.h"
#include "PrimitiveRenderer.h"

Point2D::Point2D()
{
    x = 0.0f;
    y = 0.0f;
}

Point2D::Point2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Point2D::GetX() const
{
    return x;
}

float Point2D::GetY() const
{
    return y;
}

void Point2D::SetX(float x)
{
    this->x = x;
}

void Point2D::SetY(float y)
{
    this->y = y;
}

void Point2D::SetPosition(
    float x,
    float y)
{
    this->x = x;
    this->y = y;
}

void Point2D::Draw(
    PrimitiveRenderer& renderer,
    SDL_Color color) const
{
    renderer.DrawPoint(
        x,
        y,
        color);
}