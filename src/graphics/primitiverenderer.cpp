

#include "PrimitiveRenderer.h"
#include<math.h>
#include <algorithm>
#include <cmath>
#include <queue>

PrimitiveRenderer::PrimitiveRenderer(SDL_Renderer* r)
{
    renderer = r;
}
SDL_Color PrimitiveRenderer::GetPixelColor(SDL_Surface* surface, int x, int y)
{
    if (!surface || x < 0 || x >= surface->w || y < 0 || y >= surface->h)
    {
        return SDL_Color{0, 0, 0, 0};
    }

    const Uint8* pixelAddress = static_cast<const Uint8*>(surface->pixels)
                                + y * surface->pitch
                                + x * SDL_BYTESPERPIXEL(surface->format);

    Uint32 pixelValue = 0;
    int bpp = SDL_BYTESPERPIXEL(surface->format);

    switch (bpp)
    {
        case 1:
            pixelValue = *pixelAddress;
            break;
        case 2:
            pixelValue = *reinterpret_cast<const Uint16*>(pixelAddress);
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                pixelValue = (pixelAddress[0] << 16) | (pixelAddress[1] << 8) | pixelAddress[2];
            else
                pixelValue = pixelAddress[0] | (pixelAddress[1] << 8) | (pixelAddress[2] << 16);
            break;
        case 4:
            pixelValue = *reinterpret_cast<const Uint32*>(pixelAddress);
            break;
        default:
            return SDL_Color{0, 0, 0, 0};
    }

    SDL_Color color;
    const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(surface->format);
    SDL_GetRGBA(pixelValue, details, nullptr, &color.r, &color.g, &color.b, &color.a);

    return color;
}

void PrimitiveRenderer::SetPixelColor(SDL_Surface* surface, int x, int y, SDL_Color color)
{
    if (!surface || x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        return;

    Uint8* pixelAddress = static_cast<Uint8*>(surface->pixels)
                          + y * surface->pitch
                          + x * SDL_BYTESPERPIXEL(surface->format);

    const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(surface->format);
    Uint32 pixelValue = SDL_MapRGBA(details, nullptr, color.r, color.g, color.b, color.a);

    int bpp = SDL_BYTESPERPIXEL(surface->format);
    switch (bpp)
    {
        case 1:
            *pixelAddress = static_cast<Uint8>(pixelValue);
            break;
        case 2:
            *reinterpret_cast<Uint16*>(pixelAddress) = static_cast<Uint16>(pixelValue);
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                pixelAddress[0] = (pixelValue >> 16) & 0xFF;
                pixelAddress[1] = (pixelValue >> 8) & 0xFF;
                pixelAddress[2] = pixelValue & 0xFF;
            } else {
                pixelAddress[0] = pixelValue & 0xFF;
                pixelAddress[1] = (pixelValue >> 8) & 0xFF;
                pixelAddress[2] = (pixelValue >> 16) & 0xFF;
            }
            break;
        case 4:
            *reinterpret_cast<Uint32*>(pixelAddress) = pixelValue;
            break;
    }
}
bool PrimitiveRenderer::ColorsMatch(const SDL_Color& c1, const SDL_Color& c2) {
    return c1.r == c2.r &&
           c1.g == c2.g &&
           c1.b == c2.b &&
           c1.a == c2.a;
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

void PrimitiveRenderer::DrawPolygon(const std::vector<Point2D>& points, SDL_Color color) {
    size_t n = points.size();

    if (n < 3)
    {
        return;
    }

    for (size_t i = 0; i < n; ++i)
    {
        Point2D p1 = points[i];
        Point2D q1 = points[(i + 1) % n];

        for (size_t j = i + 1; j < n; ++j)
        {
            if (j == (i + 1) % n) continue;
            if (i == 0 && j == n - 1) continue;

            Point2D p2 = points[j];
            Point2D q2 = points[(j + 1) % n];

            if (LineSegment::DoIntersect(p1, q1, p2, q2))
            {
                return;
            }
        }
    }

    DrawPolilane(points, true, color);
}

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
void PrimitiveRenderer::DrawFilledCircle(float x0, float y0, int r, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    const float PI = 3.1415926535f;
    float step = (r > 0) ? (1.0f / static_cast<float>(r)) : 0.01f;

    for (float alpha = 0.0f; alpha <= PI / 2.0f; alpha += step)
    {
        float dx = std::round(r * std::cos(alpha));
        float dy = std::round(r * std::sin(alpha));

        DrawLineSDL(x0 - dx, y0 + dy, x0 + dx, y0 + dy, color);
        DrawLineSDL(x0 - dx, y0 - dy, x0 + dx, y0 - dy, color);
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
void PrimitiveRenderer::DrawFilledEllipse(float x0, float y0, int r_x, int r_y, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    const float PI = 3.1415926535f;
    int maxR = std::max(r_x, r_y);
    float step = (maxR > 0) ? (1.0f / static_cast<float>(maxR)) : 0.01f;

    for (float alpha = 0.0f; alpha <= PI / 2.0f; alpha += step)
    {
        float dx = std::round(r_x * std::cos(alpha));
        float dy = std::round(r_y * std::sin(alpha));

        DrawLineSDL(x0 - dx, y0 + dy, x0 + dx, y0 + dy, color);
        DrawLineSDL(x0 - dx, y0 - dy, x0 + dx, y0 - dy, color);
    }
}
void PrimitiveRenderer::BoundaryFill(Point2D seed, SDL_Color fillColor, SDL_Color boundaryColor) {
    int w = 0, h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    if (ColorsMatch(fillColor, boundaryColor)) return;
    SDL_Surface* snapshot = SDL_RenderReadPixels(renderer, nullptr);
    if (!snapshot) return;
    std::queue<Point2D> q;
    q.push(seed);
    while (!q.empty())
    {
        Point2D p = q.front();
        q.pop();

        int x = static_cast<int>(p.GetX());
        int y = static_cast<int>(p.GetY());
        if (x < 0 || x >= snapshot->w || y < 0 || y >= snapshot->h)
            continue;

        SDL_Color current = GetPixelColor(snapshot,x, y);

        if (ColorsMatch(current, fillColor) || ColorsMatch(current, boundaryColor))
            continue;

        DrawPoint(x, y, fillColor);
        SetPixelColor(snapshot, x, y, fillColor);
        q.emplace(x, y - 1);
        q.emplace(x, y + 1);
        q.emplace(x - 1, y);
        q.emplace(x + 1, y);
    }
    SDL_DestroySurface(snapshot);
}

void PrimitiveRenderer::FloodFill(Point2D seed, SDL_Color fillColor, SDL_Color backgroundColor) {
    int w = 0, h = 0;
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    if (ColorsMatch(fillColor, backgroundColor)) return;
    SDL_Surface* snapshot = SDL_RenderReadPixels(renderer, nullptr);
    if (!snapshot) return;
    std::queue<Point2D> q;
    q.push(seed);
    while (!q.empty())
    {
        Point2D p = q.front();
        q.pop();

        int x = static_cast<int>(p.GetX());
        int y = static_cast<int>(p.GetY());
        if (x < 0 || x >= snapshot->w || y < 0 || y >= snapshot->h)
            continue;

        SDL_Color current = GetPixelColor(snapshot,x, y);

        if (ColorsMatch(current, fillColor) || !ColorsMatch(current, backgroundColor))
            continue;

        DrawPoint(x, y, fillColor);
        SetPixelColor(snapshot, x, y, fillColor);
        q.emplace(x, y - 1);
        q.emplace(x, y + 1);
        q.emplace(x - 1, y);
        q.emplace(x + 1, y);
    }
    SDL_DestroySurface(snapshot);
}