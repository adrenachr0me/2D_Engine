

#ifndef ENGINE2D_LINESEGMENT_H
#define ENGINE2D_LINESEGMENT_H
#pragma once

#include <SDL3/SDL.h>
#include "Point2D.h"
#include "PrimitiveRenderer.h"

class LineSegment
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

    void Draw(
        PrimitiveRenderer& renderer,
        SDL_Color color,
        bool incremental = false) const;
};

#endif //ENGINE2D_LINESEGMENT_H