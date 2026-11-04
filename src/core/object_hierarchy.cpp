#include "core/object_hierarchy.h"
#include "graphics/point2D.h"
#include "graphics/primitiverenderer.h"

BitmapObject::BitmapObject() : position(new Point2D(0.0f, 0.0f)), rotationAngle(0.0f), scaleFactor(1.0f){}
BitmapObject::BitmapObject(float x, float y) : position(new Point2D(x, y)), rotationAngle(0.0f), scaleFactor(1.0f) {}
BitmapObject::~BitmapObject(){
    delete position;
    position = nullptr;
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
}

void BitmapObject::Scale(float kx, float ky, const Point2D& center) {
    if (position) {
        position->Scale(kx, ky, center);
    }
    scaleFactor *= kx;
}

void BitmapObject::Draw(PrimitiveRenderer& renderer) {
    float angleDeg = rotationAngle * (180.0f / 3.1415926535f);

    for (const auto* bmp : bitmaps)
    {
        if (bmp)
        {
            renderer.DrawBitmap(*bmp, position->GetX(), position->GetY(), scaleFactor, angleDeg);
        }
    }
}

SpriteObject::SpriteObject()
    : BitmapObject()
{
}

SpriteObject::SpriteObject(float x, float y)
    : BitmapObject(x, y)
{
}

std::size_t SpriteObject::AddAnimation(
    const std::vector<const BitmapHandler*>& frames)
{
    animations.push_back(frames);
    return animations.size() - 1;
}

void SpriteObject::SetAnimation(std::size_t index)
{
    if(index >= animations.size())
        return;

    currentAnimation = index;
    currentFrame = 0;
    timeAccumulator = 0.0f;
}

void SpriteObject::SetFrameDuration(float seconds)
{
    frameDuration = seconds;
}

void SpriteObject::Play()
{
    playing = true;
}

void SpriteObject::Stop()
{
    playing = false;
    currentFrame = 0;
}

void SpriteObject::Animate(float deltaTime)
{
    if(!playing)
        return;

    if(animations.empty())
        return;

    auto& animation = animations[currentAnimation];

    if(animation.empty())
        return;

    timeAccumulator += deltaTime;

    if(timeAccumulator >= frameDuration)
    {
        timeAccumulator = 0.0f;

        currentFrame++;

        if(currentFrame >= animation.size())
            currentFrame = 0;
    }
}

void SpriteObject::Draw(PrimitiveRenderer& renderer)
{
    if(animations.empty())
        return;

    auto& animation = animations[currentAnimation];

    if(animation.empty())
        return;

    const BitmapHandler* bitmap = animation[currentFrame];

    if(!bitmap)
        return;

    float angleDeg =
        rotationAngle * (180.0f / 3.1415926535f);

    renderer.DrawBitmap(
        *bitmap,
        position->GetX(),
        position->GetY(),
        scaleFactor,
        angleDeg
    );
}