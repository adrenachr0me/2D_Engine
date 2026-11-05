
#ifndef ENGINE2D_COLLIDER_H
#define ENGINE2D_COLLIDER_H
#pragma once
#include "graphics/point2D.h"
struct AABB {
    float x{0.0f};
    float y{0.0f};
    float w{0.0f};
    float h{0.0f};

    AABB() = default;
    AABB(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
    bool Intersects(const AABB& other) const {
        return (x < other.x + other.w &&
                x + w > other.x &&
                y < other.y + other.h &&
                y + h > other.y);
    }

    bool Contains(float px, float py) const{
        return (px >= x && px <= x + w &&
                py >= y && py <= y + h);
    }
};
#endif //ENGINE2D_COLLIDER_H