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