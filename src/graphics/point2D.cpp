

#include "../../include/graphics/point2D.h"
#include "PrimitiveRenderer.h"
#include <cmath>

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

void Point2D::Draw(PrimitiveRenderer& renderer)
{
    renderer.DrawPoint(
        x,
        y,
        color);
}
void Point2D::Translate(float tx, float ty)
{
    x += tx;
    y += ty;
}

void Point2D::Rotate(float alpha)
{
    float cosA = std::cos(alpha);
    float sinA = std::sin(alpha);
    float newX = x * cosA - y * sinA;
    float newY = x * sinA + y * cosA;
    x = newX;
    y = newY;
}

void Point2D::Rotate(float alpha, const Point2D& center)
{
    float cosA = std::cos(alpha);
    float sinA = std::sin(alpha);
    float dx = x - center.GetX();
    float dy = y - center.GetY();

    x = center.GetX() + (dx * cosA - dy * sinA);
    y = center.GetY() + (dx * sinA + dy * cosA);
}

void Point2D::Scale(float kx, float ky)
{
    x *= kx;
    y *= ky;
}

void Point2D::Scale(float kx, float ky, const Point2D& center)
{
    x = center.GetX() + (x - center.GetX()) * kx;
    y = center.GetY() + (y - center.GetY()) * ky;
}