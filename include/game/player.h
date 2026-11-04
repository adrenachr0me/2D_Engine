
#ifndef ENGINE2D_PLAYER_H
#define ENGINE2D_PLAYER_H
#pragma once
#include "core/object_hierarchy.h"
#include "graphics/linesegment.h"
#include "graphics/point2D.h"
#include <vector>
#include "core/timer.h"
#include "system/input.h"
class Player: public UpdatableObject, public SpriteObject {
private:
    float speed{200.0f};
    std::vector<BitmapHandler> sprites;
    bool spritesLoaded{false};
    const Input& input;

public:
    Player(float startX, float startY, const Input& inputModule);
    void Update(float deltaTime) override;
    void Draw(PrimitiveRenderer& renderer) override;
    bool HasSprites() const { return spritesLoaded; }
};
#endif //ENGINE2D_PLAYER_H
