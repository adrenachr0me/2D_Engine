
#ifndef ENGINE2D_OBJECT_HIERARCHY_H
#define ENGINE2D_OBJECT_HIERARCHY_H
#pragma once

#include <SDL3/SDL.h>

class PrimitiveRenderer;
class Point2D;

class GameObject
{
public:
    virtual ~GameObject() = default;
};

class UpdatableObject : virtual public GameObject
{
public:
    virtual ~UpdatableObject() = default;
    virtual void Update(float deltaTime) = 0;
};

class DrawableObject : virtual public GameObject
{
public:
    virtual ~DrawableObject() = default;
    virtual void Draw(PrimitiveRenderer& renderer) = 0;
};

class TransformableObject : virtual public GameObject
{
public:
    virtual ~TransformableObject() = default;

    virtual void Translate(float tx, float ty) = 0;
    virtual void Rotate(float alpha) = 0;
    virtual void Rotate(float alpha, const Point2D& center) = 0;
    virtual void Scale(float kx, float ky) = 0;
    virtual void Scale(float kx, float ky, const Point2D& center) = 0;
};

class ShapeObject : public DrawableObject, public TransformableObject
{
protected:
    SDL_Color color{255, 255, 255, 255};

public:
    virtual ~ShapeObject() = default;

    void SetColor(SDL_Color c) { color = c; }
    SDL_Color GetColor() const { return color; }
};
#endif