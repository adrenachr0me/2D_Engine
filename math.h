//
// Created by telep on 10/10/2026.
//

#ifndef ENGINE2D_MATH_H
#define ENGINE2D_MATH_H
#pragma once
#include "cmath"
struct coordinates {
    float x = 0.0;
    float y = 0.0;
    coordinates() = default;
    coordinates(float x,float y) : x(x), y(y){}

    coordinates operator+(const coordinates& other) const {return coordinates(x + other.x, y + other.y);}
    coordinates operator-(const coordinates& other) const {return coordinates(x - other.x, y - other.y);}
    coordinates operator*(float scalar) const {return coordinates(x * scalar, y * scalar);}
    coordinates operator/(float scalar) const {return coordinates(x / scalar, y / scalar);}
    coordinates operator+=(const coordinates& other) {(x += other.x, y += other.y); return *this;}
    coordinates operator-=(const coordinates& other) {(x -= other.x, y -= other.y); return *this;}
};
#endif //ENGINE2D_MATH_H