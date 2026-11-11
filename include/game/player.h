
#ifndef ENGINE2D_PLAYER_H
#define ENGINE2D_PLAYER_H
#pragma once
#include "core/object_hierarchy.h"
#include "graphics/linesegment.h"
#include "graphics/point2D.h"
#include <vector>
#include "core/timer.h"
#include "system/input.h"
#include "physics/collider.h"
#include "world/tilemap.h"
class Player: public UpdatableObject, public SpriteObject {
private:
    float speed{200.0f};
    std::vector<BitmapHandler> sprites;
    bool spritesLoaded{false};
    const Input& input;
    float colliderW{24.0f};
    float colliderH{28.0f};

public:
    Player(float startX, float startY, const Input& inputModule);
    void Update(float deltaTime, const Tilemap& map);
    void Update(float deltaTime) override;
    void Draw(PrimitiveRenderer& renderer) override;

    void Draw(PrimitiveRenderer &renderer, const Camera &camera) override;
    void SetPosition(float x, float y);
    bool HasSprites() const { return spritesLoaded; }
    AABB GetCollider() const {
        float currentW = colliderW * scaleFactor;
        float currentH = colliderH * scaleFactorY;
        return AABB(GetX() - currentW / 2.0f, GetY() - currentH / 2.0f, currentW, currentH);
    }
};
#endif //ENGINE2D_PLAYER_H
