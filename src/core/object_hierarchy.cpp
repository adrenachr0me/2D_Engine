#include "core/object_hierarchy.h"
#include "graphics/point2D.h"
#include "graphics/primitiverenderer.h"
#include <cmath>

BitmapObject::BitmapObject() : position(new Point2D(0.0f, 0.0f)), rotationAngle(0.0f), scaleFactor(1.0f){}
BitmapObject::BitmapObject(float x, float y) : position(new Point2D(x, y)), rotationAngle(0.0f), scaleFactor(1.0f) {}
BitmapObject::~BitmapObject(){
    delete position;
    position = nullptr;
}
BitmapObject::BitmapObject(BitmapObject&& other) noexcept
    : position(other.position),
      rotationAngle(other.rotationAngle),
      scaleFactor(other.scaleFactor),
      scaleFactorY(other.scaleFactorY),
      bitmaps(std::move(other.bitmaps))
{
    other.position = nullptr;
    other.rotationAngle = 0.0f;
    other.scaleFactor = 1.0f;
    other.scaleFactorY = 1.0f;
}

BitmapObject& BitmapObject::operator=(BitmapObject&& other) noexcept
{
    if (this != &other)
    {
        delete position;

        position = other.position;
        rotationAngle = other.rotationAngle;
        scaleFactor = other.scaleFactor;
        scaleFactorY = other.scaleFactorY;
        bitmaps = std::move(other.bitmaps);

        other.position = nullptr;
        other.rotationAngle = 0.0f;
        other.scaleFactor = 1.0f;
        other.scaleFactorY = 1.0f;
    }
    return *this;
}

float BitmapObject::GetX() const
{
    return position ? position->GetX() : 0.0f;
}

float BitmapObject::GetY() const
{
    return position ? position->GetY() : 0.0f;
}

void BitmapObject::AddBitmap(const BitmapHandler* bitmap) {
    if (bitmap) {
        bitmaps.push_back(bitmap);
    }
}

void BitmapObject::Translate(float tx, float ty) {
    if (position) {
        position->Translate(tx,ty);
    }
}

void BitmapObject::Rotate(float alpha) {
    rotationAngle += alpha;
}

void BitmapObject::Rotate(float alpha,const Point2D& center) {
    if (position) {
        position->Rotate(alpha, center);
    }
    rotationAngle += alpha;
}

void BitmapObject::Scale(float kx, float ky) {
    scaleFactor *= kx;
    scaleFactorY *= ky;
}

void BitmapObject::Scale(float kx, float ky, const Point2D& center) {
    if (position) {
        position->Scale(kx, ky, center);
    }
    scaleFactor *= kx;
    scaleFactorY *= ky;
}

void BitmapObject::Draw(PrimitiveRenderer& renderer) {
    float angleDeg = rotationAngle * (180.0f / 3.1415926535f);

    for (const auto* bmp : bitmaps)
    {
        if (bmp)
        {
            renderer.DrawBitmap(*bmp, position->GetX(), position->GetY(), scaleFactor, scaleFactorY, angleDeg);
        }
    }
}

bool SpriteObject::SetAnimation(std::size_t first, std::size_t count) {
    if (count == 0 || first >= bitmaps.size() || count > bitmaps.size() - first)
        return false;
    if (firstFrame != first || frameCount != count) {
        firstFrame = first;
        frameCount = count;
        currentFrame = 0;
        elapsed = 0.0f;
    }
    return true;
}

void SpriteObject::SetAnimating(bool enabled) {
    animating = enabled;
    if (!enabled) {
        currentFrame = 0;
        elapsed = 0.0f;
    }
}

void SpriteObject::Animate(float deltaTime) {
    if (!animating || frameCount == 0 || !std::isfinite(deltaTime) || deltaTime <= 0.0f)
        return;
    const double total = static_cast<double>(elapsed) + deltaTime;
    const double steps = std::floor(total / frameDuration);
    elapsed = static_cast<float>(std::fmod(total, frameDuration));
    currentFrame = static_cast<std::size_t>(std::fmod(currentFrame + steps, frameCount));
}

void SpriteObject::Draw(PrimitiveRenderer& renderer) {
    if (frameCount == 0)
        return;
    const auto* bitmap = bitmaps[firstFrame + currentFrame];
    if (bitmap) {
        const float angleDeg = rotationAngle * (180.0f / 3.1415926535f);
        renderer.DrawBitmap(*bitmap, position->GetX(), position->GetY(), scaleFactor, scaleFactorY, angleDeg);
    }
}

void SpriteObject::Draw(PrimitiveRenderer& renderer, const Camera& camera) {
    if (frameCount == 0 || !position)
        return;

    const auto* bitmap = bitmaps[firstFrame + currentFrame];
    if (bitmap) {
        const float angleDeg = rotationAngle * (180.0f / 3.1415926535f);
        float screenX = camera.WorldToScreenX(position->GetX());
        float screenY = camera.WorldToScreenY(position->GetY());

        renderer.DrawBitmap(*bitmap, screenX, screenY, scaleFactor, scaleFactorY, angleDeg);
    }
}
