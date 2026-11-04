#ifndef ENGINE2D_PLAYER_H
#define ENGINE2D_PLAYER_H
#pragma once
#include "core/object_hierarchy.h"
#include "graphics/linesegment.h"
#include "graphics/point2D.h"
#include <vector>
#include "core/timer.h"
#include "system/input.h"

// Upewnij się, że ten nagłówek jest dołączony, by klasa znała BitmapHandler
#include "graphics/bitmap_handler.h" 

class Player: public SpriteObject, public UpdatableObject{
private:

    float speed{300.0f};
    const Input& input;
    std::size_t activeAnimID{0};


public:
    Player(float startX, float startY, const Input& inputModule);
    ~Player();
    void Update(float deltaTime) override;

    std::vector<BitmapHandler*> downFrames;
    std::vector<BitmapHandler*> upFrames;
    std::vector<BitmapHandler*> leftFrames;
    std::vector<BitmapHandler*> rightFrames;

    std::size_t downAnimation;
    std::size_t upAnimation;
    std::size_t leftAnimation;
    std::size_t rightAnimation;
};
#endif //ENGINE2D_PLAYER_H
