

#include "LineSegment.h"

LineSegment::LineSegment()
{
}

LineSegment::LineSegment(
    const Point2D& start,
    const Point2D& end)
    : startPoint(start),
      endPoint(end)
{
}

Point2D LineSegment::GetStartPoint() const
{
    return startPoint;
}

Point2D LineSegment::GetEndPoint() const
{
    return endPoint;
}

void LineSegment::SetStartPoint(
    const Point2D& start)
{
    startPoint = start;
}

void LineSegment::SetEndPoint(
    const Point2D& end)
{
    endPoint = end;
}

void LineSegment::Draw(
    PrimitiveRenderer& renderer,
    SDL_Color color,
    bool incremental) const
{
    if (incremental)
    {
        renderer.DrawLineIncremental(
            startPoint.GetX(),
            startPoint.GetY(),
            endPoint.GetX(),
            endPoint.GetY(),
            color);
    }
    else
    {
        renderer.DrawLineSDL(
            startPoint.GetX(),
            startPoint.GetY(),
            endPoint.GetX(),
            endPoint.GetY(),
            color);
    }
}