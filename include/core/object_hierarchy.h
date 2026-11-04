
#ifndef ENGINE2D_OBJECT_HIERARCHY_H
#define ENGINE2D_OBJECT_HIERARCHY_H
#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "graphics/bitmap_handler.h"
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
class AnimatedObject : virtual public GameObject{
public:
    virtual ~AnimatedObject() = default;
    virtual void Animate(float deltaTime) = 0;
};

class BitmapObject : public DrawableObject, public TransformableObject {
protected:
    Point2D* position{nullptr};
    float rotationAngle{0.0f};
    float scaleFactor{1.0f};
    float scaleFactorY{1.0f};
    std::vector<const BitmapHandler*> bitmaps;
public:
    BitmapObject();
    BitmapObject(float x, float y);
    virtual ~BitmapObject();
    BitmapObject(const BitmapObject&) = delete;
    BitmapObject& operator=(const BitmapObject&) = delete;
    void AddBitmap(const BitmapHandler* bitmap);
    void Translate(float tx, float ty) override;
    void Rotate(float alpha) override;
    void Rotate(float alpha, const Point2D& center) override;
    void Scale(float kx, float ky) override;
    void Scale(float kx, float ky, const Point2D& center) override;

    void Draw(PrimitiveRenderer& renderer) override;

};

class SpriteObject : public BitmapObject, public AnimatedObject {
private:
    std::size_t firstFrame{0};
    std::size_t frameCount{0};
    std::size_t currentFrame{0};
    float elapsed{0.0f};
    float frameDuration{0.125f};
    bool animating{false};
public:
    using BitmapObject::BitmapObject;
    bool SetAnimation(std::size_t first, std::size_t count);
    void SetAnimating(bool enabled);
    void Animate(float deltaTime) override;
    void Draw(PrimitiveRenderer& renderer) override;
    std::size_t GetCurrentFrame() const { return currentFrame; }
};
#endif
