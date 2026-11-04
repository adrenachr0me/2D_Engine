#include "../../include/game/player.h"

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
