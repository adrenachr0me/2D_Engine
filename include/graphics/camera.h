

#ifndef ENGINE2D_CAMERA_H
#define ENGINE2D_CAMERA_H
#pragma once
#include <algorithm>
struct Camera {
private:
    float x{0.0f};
    float y{0.0f};
    int viewPortWidth{1280};
    int viewPortHeight{720};
public:
    Camera() = default;
    Camera(int width, int height) : viewPortWidth(width), viewPortHeight(height){}
    void Follow(float targetX, float targetY) {
        x = targetX - viewPortWidth / 2.0f;
        y = targetY - viewPortHeight / 2.0f;
    }

    void ClampToBounds(float worldWidth, float worldHeight) {
        if (worldWidth > viewPortWidth) {
            x = std::clamp(x, 0.0f, worldWidth - viewPortWidth);
        }
        else {
            x = (worldWidth - viewPortWidth) / 2.0f;
        }
        if (worldHeight > viewPortHeight)
            y = std::clamp(y, 0.0f, worldHeight - viewPortHeight);
        else
            y = (worldHeight - viewPortHeight) / 2.0f;
    }
    float WorldToScreenX(float worldX) const { return worldX - x; }
    float WorldToScreenY(float worldY) const { return worldY - y; }
    float ScreenToWorldX(float screenX) const { return screenX + x; }
    float ScreenToWorldY(float screenY) const { return screenY + y; }
    float GetX() const { return x; }
    float GetY() const { return y; }
    int GetViewportWidth() const { return viewPortWidth; }
    int GetViewportHeight() const { return viewPortHeight; }
};
#endif //ENGINE2D_CAMERA_H