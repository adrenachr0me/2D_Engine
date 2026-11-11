#include "../../include/game/player.h"
#include <cmath>
#include "primitiverenderer.h"

Player::Player(float startX, float startY,const Input& inputModule) : SpriteObject(startX, startY), sprites(16), input(inputModule) {
    const char* basePath = SDL_GetBasePath();
    const std::string filepath = std::string(basePath ? basePath : "") + "assets/player.bmp";
    for (int row = 0; row < 4; ++row) {
        for (int frame = 0; frame < 4; ++frame) {
            auto& bitmap = sprites[row * 4 + frame];
            const SDL_Rect area{frame * 32, row * 40, 32, 40};
            if (!bitmap.LoadFragment(filepath, area))
                return;
            AddBitmap(&bitmap);
        }
    }
    SetAnimation(0, 4);
    Scale(1.5f, 1.5f);
    spritesLoaded = true;
}
void Player::Update(float deltaTime, const Tilemap& map) {
    float vx = 0.0f;
    float vy = 0.0f;

    if (input.IsKeyDown(SDL_SCANCODE_W) || input.IsKeyDown(SDL_SCANCODE_UP))    vy -= speed;
    if (input.IsKeyDown(SDL_SCANCODE_S) || input.IsKeyDown(SDL_SCANCODE_DOWN))  vy += speed;
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT))  vx -= speed;
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT)) vx += speed;

    if (vx != 0.0f && vy != 0.0f) {
        vx *= 0.70710678f;
        vy *= 0.70710678f;
    }

    float dx = vx * deltaTime;
    float dy = vy * deltaTime;

    const int tSize = map.GetTileSize();

    if (dx != 0.0f) {
        Translate(dx, 0.0f);
        AABB box = GetCollider();
        float halfW = box.w / 2.0f;
        int startTileX = static_cast<int>(std::floor(box.x / tSize));
        int endTileX   = static_cast<int>(std::floor((box.x + box.w - 0.01f) / tSize));
        int startTileY = static_cast<int>(std::floor(box.y / tSize));
        int endTileY   = static_cast<int>(std::floor((box.y + box.h - 0.01f) / tSize));

        for (int ty = startTileY; ty <= endTileY; ++ty) {
            for (int tx = startTileX; tx <= endTileX; ++tx) {
                if (map.IsSolidAtTile(tx, ty)) {
                    AABB tileBox(static_cast<float>(tx * tSize), static_cast<float>(ty * tSize),
                                 static_cast<float>(tSize), static_cast<float>(tSize));
                    if (box.Intersects(tileBox)) {
                        if (dx > 0.0f) {
                            SetPosition(tileBox.x - halfW, GetY());
                        } else if (dx < 0.0f) {
                            SetPosition(tileBox.x + tileBox.w + halfW, GetY());
                        }
                        box = GetCollider();
                    }
                }
            }
        }
    }

    if (dy != 0.0f) {
        Translate(0.0f, dy);
        AABB box = GetCollider();
        float halfH = box.h / 2.0f;
        int startTileX = static_cast<int>(std::floor(box.x / tSize));
        int endTileX   = static_cast<int>(std::floor((box.x + box.w - 0.01f) / tSize));
        int startTileY = static_cast<int>(std::floor(box.y / tSize));
        int endTileY   = static_cast<int>(std::floor((box.y + box.h - 0.01f) / tSize));

        for (int ty = startTileY; ty <= endTileY; ++ty) {
            for (int tx = startTileX; tx <= endTileX; ++tx) {
                if (map.IsSolidAtTile(tx, ty)) {
                    AABB tileBox(static_cast<float>(tx * tSize), static_cast<float>(ty * tSize),
                                 static_cast<float>(tSize), static_cast<float>(tSize));
                    if (box.Intersects(tileBox)) {
                        if (dy > 0.0f) {
                            SetPosition(GetX(), tileBox.y - halfH);
                        } else if (dy < 0.0f) {
                            SetPosition(GetX(), tileBox.y + tileBox.h + halfH);
                        }
                        box = GetCollider();
                    }
                }
            }
        }
    }

    if (dx != 0.0f || dy != 0.0f) {
        const int row = dy < 0.0f ? 3 : dy > 0.0f ? 0 : dx < 0.0f ? 1 : 2;
        SetAnimation(row * 4, 4);
        SetAnimating(true);
        Animate(deltaTime);
    } else {
        SetAnimating(false);
    }

    float rotSpeed = 3.0f;
    if (input.IsKeyDown(SDL_SCANCODE_Q)) Rotate(-rotSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_E)) Rotate(rotSpeed * deltaTime);

    float scaleSpeed = 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_R)) Scale(1.0f + scaleSpeed * deltaTime, 1.0f + scaleSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_F)) Scale(1.0f - scaleSpeed * deltaTime, 1.0f - scaleSpeed * deltaTime);
}
void Player::Update(float deltaTime) {
    float dx = 0.0f;
    float dy = 0.0f;
    if (input.IsKeyDown(SDL_SCANCODE_W) || input.IsKeyDown(SDL_SCANCODE_UP))    dy -= speed * deltaTime;
    if (input.IsKeyDown(SDL_SCANCODE_S) || input.IsKeyDown(SDL_SCANCODE_DOWN))  dy += speed * deltaTime;
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT))  dx -= speed * deltaTime;
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT)) dx += speed * deltaTime;
    if (dx != 0.0f || dy != 0.0f)
    {
        Translate(dx, dy);
        const int row = dy < 0.0f ? 3 : dy > 0.0f ? 0 : dx < 0.0f ? 1 : 2;
        SetAnimation(row * 4, 4);
        SetAnimating(true);
        Animate(deltaTime);
    }
    else {
        SetAnimating(false);
    }
    float rotSpeed = 3.0f;
    if (input.IsKeyDown(SDL_SCANCODE_Q)) Rotate(-rotSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_E)) Rotate(rotSpeed * deltaTime);
    float scaleSpeed = 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_R)) Scale(1.0f + scaleSpeed * deltaTime, 1.0f + scaleSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_F)) Scale (1.0f - scaleSpeed * deltaTime, 1.0f - scaleSpeed * deltaTime);
}
void Player::Draw(PrimitiveRenderer& renderer) {
    SpriteObject::Draw(renderer);
}
void Player::Draw(PrimitiveRenderer& renderer, const Camera& camera) {
    SpriteObject::Draw(renderer, camera);
}

void Player::SetPosition(float x, float y) {
    if (position) {
        position->SetPosition(x, y);
    }
}