

#include "LineSegment.h"
#include "primitiverenderer.h"
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
void LineSegment::Draw(PrimitiveRenderer& renderer)
{
    Draw(renderer, false, color);
}
void LineSegment::Draw(PrimitiveRenderer& renderer,bool incremental, SDL_Color color) const
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
void LineSegment::Translate(float tx, float ty)
{
    startPoint.Translate(tx, ty);
    endPoint.Translate(tx, ty);
}

void LineSegment::Rotate(float alpha)
{
    startPoint.Rotate(alpha);
    endPoint.Rotate(alpha);
}

void LineSegment::Rotate(float alpha, const Point2D& center)
{
    startPoint.Rotate(alpha, center);
    endPoint.Rotate(alpha, center);
}

void LineSegment::Scale(float kx, float ky)
{
    startPoint.Scale(kx, ky);
    endPoint.Scale(kx, ky);
}

void LineSegment::Scale(float kx, float ky, const Point2D& center)
{
    startPoint.Scale(kx, ky, center);
    endPoint.Scale(kx, ky, center);
}
float Orientation(const Point2D& a, const Point2D& b, const Point2D& c)
{
    return (b.GetX() - a.GetX()) * (c.GetY() - a.GetY()) -
           (b.GetY() - a.GetY()) * (c.GetX() - a.GetX());
}

bool OnSegment(const Point2D& p, const Point2D& q, const Point2D& r)
{
    return q.GetX() <= std::max(p.GetX(), r.GetX()) && q.GetX() >= std::min(p.GetX(), r.GetX()) &&
           q.GetY() <= std::max(p.GetY(), r.GetY()) && q.GetY() >= std::min(p.GetY(), r.GetY());
}

bool DoIntersect(const Point2D& p1, const Point2D& q1, const Point2D& p2, const Point2D& q2)
{
    float d1 = Orientation(p1, q1, p2);
    float d2 = Orientation(p1, q1, q2);
    float d3 = Orientation(p2, q2, p1);
    float d4 = Orientation(p2, q2, q1);

    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
    {
        return true;
    }

    if (d1 == 0 && OnSegment(p1, p2, q1)) return true;
    if (d2 == 0 && OnSegment(p1, q2, q1)) return true;
    if (d3 == 0 && OnSegment(p2, p1, q2)) return true;
    if (d4 == 0 && OnSegment(p2, q1, q2)) return true;

    return false;
}
