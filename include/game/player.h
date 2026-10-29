
#ifndef ENGINE2D_PLAYER_H
#define ENGINE2D_PLAYER_H
#pragma once
#include "core/object_hierarchy.h"
#include "graphics/linesegment.h"
#include "graphics/point2D.h"
#include <vector>
#include "core/timer.h"
#include "system/input.h"
class Player: public UpdatableObject, public TransformableObject, public DrawableObject {
private:
    Point2D position;
    float rotationAngle{0.0f};
    float scaleFactor{1.0f};
    float speed{200.0f};
    std::vector<LineSegment> model;
    const Input& input;

public:
    Player(float startX, float startY, const Input& inputModule);
    void Update(float deltaTime) override;
    void Draw(PrimitiveRenderer& renderer) override;
    void Translate(float tx, float ty) override;
    void Rotate(float alpha) override;
    void Rotate(float alpha, const Point2D& center) override;
    void Scale(float kx, float ky) override;
    void Scale(float kx, float ky, const Point2D& center) override;
};
#endif //ENGINE2D_PLAYER_H