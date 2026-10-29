#include "../../include/game/player.h"

Player::Player(float startX, float startY,const Input& inputModule) : position(startX, startY), input(inputModule) {
    float halfSize = 15.0f;
    Point2D nose(0.0f, -30.0f);
    Point2D leftWing(-20.0f, 20.0f);
    Point2D rightWing(20.0f, 20.0f);
    Point2D centerBack(0.0f, 10.0f);

    model.emplace_back(nose, leftWing);
    model.emplace_back(leftWing, centerBack);
    model.emplace_back(centerBack, rightWing);
    model.emplace_back(rightWing, nose);
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
    }
    float rotSpeed = 3.0f;
    if (input.IsKeyDown(SDL_SCANCODE_Q)) Rotate(-rotSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_E)) Rotate(rotSpeed * deltaTime);
    float scaleSpeed = 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_R)) Scale(1.0f + scaleSpeed * deltaTime, 1.0f + scaleSpeed * deltaTime);
    if (input.IsKeyDown(SDL_SCANCODE_F)) Scale (1.0f - scaleSpeed * deltaTime, 1.0f - scaleSpeed * deltaTime);
}
void Player::Draw(PrimitiveRenderer& renderer) {
    for (const auto& segment : model) {
        LineSegment worldSegment = segment;

        worldSegment.Scale(scaleFactor, scaleFactor);
        worldSegment.Rotate(rotationAngle);

        worldSegment.Translate(position.GetX(), position.GetY());
        worldSegment.Draw(renderer);
    }

}
void Player::Translate(float tx, float ty)
{
    position.Translate(tx, ty);
}

void Player::Rotate(float alpha)
{
    rotationAngle += alpha;
}

void Player::Rotate(float alpha, const Point2D& center)
{
    position.Rotate(alpha, center);
    rotationAngle += alpha;
}

void Player::Scale(float kx, float ky)
{
    scaleFactor *= kx;
}

void Player::Scale(float kx, float ky, const Point2D& center)
{
    position.Scale(kx, ky, center);
    scaleFactor *= kx;
}

