

#ifndef ENGINE2D_LINESEGMENT_H
#define ENGINE2D_LINESEGMENT_H
#pragma once

#include <SDL3/SDL.h>
#include "point2D.h"

class PrimitiveRenderer;
class LineSegment:public ShapeObject
{
private:
    Point2D startPoint;
    Point2D endPoint;

public:
    LineSegment();

    LineSegment(
        const Point2D& start,
        const Point2D& end);

    Point2D GetStartPoint() const;
    Point2D GetEndPoint() const;

    void SetStartPoint(
        const Point2D& start);

    void SetEndPoint(
        const Point2D& end);
    float Orientation(const Point2D& a, const Point2D& b, const Point2D& c);
    bool OnSegment(const Point2D& p, const Point2D& q, const Point2D& r);
    static bool DoIntersect(const Point2D& p1, const Point2D& q1, const Point2D& p2, const Point2D& q2);
    void Draw(PrimitiveRenderer& renderer, bool incremental, SDL_Color color) const;
    void Draw(PrimitiveRenderer& renderer) override;
    void Translate(float tx, float ty) override;
    void Rotate(float alpha) override;
    void Rotate(float alpha, const Point2D& center) override;
    void Scale(float kx, float ky) override;
    void Scale(float kx, float ky, const Point2D& center) override;
};

#endif //ENGINE2D_LINESEGMENT_H