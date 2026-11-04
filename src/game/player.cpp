#include "../../include/game/player.h"
#include <iostream>

Player::Player(float startX, float startY, const Input& inputModule)
    : SpriteObject(startX, startY), input(inputModule)
{
    Scale(4.0f, 4.0f);
    std::string basePath = "assets/player/";

    for(int i = 0; i < 9; i++)
    {
        downFrames.push_back(new BitmapHandler(basePath + "down" + std::to_string(i) + ".bmp"));
        upFrames.push_back(new BitmapHandler(basePath + "up" + std::to_string(i) + ".bmp"));
        leftFrames.push_back(new BitmapHandler(basePath + "left" + std::to_string(i) + ".bmp"));
        rightFrames.push_back(new BitmapHandler(basePath + "right" + std::to_string(i) + ".bmp"));
    }

    std::vector<const BitmapHandler*> downVec, upVec, leftVec, rightVec;
    for(int i = 0; i < 9; i++)
    {
        downVec.push_back(downFrames[i]);
        upVec.push_back(upFrames[i]);
        leftVec.push_back(leftFrames[i]);
        rightVec.push_back(rightFrames[i]);
    }

    downAnimation  = AddAnimation(downVec);
    upAnimation    = AddAnimation(upVec);
    leftAnimation  = AddAnimation(leftVec);
    rightAnimation = AddAnimation(rightVec);

    // USTAWIANIE STARTOWE:
    SetAnimation(downAnimation);
    activeAnimID = downAnimation; // Zapamiętujemy, że na start odpalamy ruch w dół
    Play();
}

Player::~Player() {
    for(int i = 0; i < 9; i++) {
        delete downFrames[i];
        delete upFrames[i];
        delete leftFrames[i];
        delete rightFrames[i];
    }
}

void Player::Update(float deltaTime) {
    float dx = 0.0f;
    float dy = 0.0f;

    bool isMovingUp    = input.IsKeyDown(SDL_SCANCODE_W) || input.IsKeyDown(SDL_SCANCODE_UP);
    bool isMovingDown  = input.IsKeyDown(SDL_SCANCODE_S) || input.IsKeyDown(SDL_SCANCODE_DOWN);
    bool isMovingLeft  = input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT);
    bool isMovingRight = input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT);

    // Domyślnie planujemy zostać przy obecnej animacji
    std::size_t nextAnimation = activeAnimID;

    if (isMovingUp) {
        dy -= speed * deltaTime;
        nextAnimation = upAnimation;
    }
    else if (isMovingDown) {
        dy += speed * deltaTime;
        nextAnimation = downAnimation;
    }

    if (isMovingLeft) {
        dx -= speed * deltaTime;
        nextAnimation = leftAnimation;
    }
    else if (isMovingRight) {
        dx += speed * deltaTime;
        nextAnimation = rightAnimation;
    }

    // Jeśli gracz się porusza
    if (dx != 0.0f || dy != 0.0f)
    {
        // Zmieniamy animację TYLKO wtedy, gdy zmienił się kierunek na klawiaturze
        if (nextAnimation != activeAnimID) {
            SetAnimation(nextAnimation);
            activeAnimID = nextAnimation; // Zapisujemy nową aktywną animację
        }

        Translate(dx, dy);
        Animate(deltaTime);
    }
    else
    {
        // EFEKT STOP: Gracz puścił klawisze, wymuszamy pozycję bazową "w dół"
        if (activeAnimID != downAnimation) {
            SetAnimation(downAnimation);
            activeAnimID = downAnimation;
        }
    }

    // Rotacja i skalowanie
    float rotSpeed = 3.0f;
    if (input.IsKeyDown(SDL_SCANCODE_Q)) Rotate(-rotSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_E)) Rotate(rotSpeed * deltaTime);

    float scaleSpeed = 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_R)) Scale(1.0f + scaleSpeed * deltaTime, 1.0f + scaleSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_F)) Scale(1.0f - scaleSpeed * deltaTime, 1.0f - scaleSpeed * deltaTime);
}





