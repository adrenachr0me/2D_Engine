#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "input.h"
#include "timer.h"
#include "error_handling.h"
#include "PrimitiveRenderer.h"
#include "../graphics/point2D.h"
#include "logs.h"
#include "game/player.h"
#include "graphics/camera.h"
#include "world/tilemap.h"
#include "physics/collider.h"
enum class GameState {
    Playing,
    Victory
};

struct Portal {
    AABB triggerBox;
    float centerX{0.0f};
    float centerY{0.0f};
    bool active{false};
};
struct Star {
    float x{0.0f};
    float y{0.0f};
    float radius{12.0f};
    bool collected{false};

    AABB GetCollider() const {
        return AABB(x - radius, y - radius, radius * 2.0f, radius * 2.0f);
    }
};
class Engine
{
private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    PrimitiveRenderer* primitiveRenderer;

    bool running;

    int width;
    int height;
    int fps;

    bool fullscreen;

    Input input;
    Timer timer;
    Player* player;
    BitmapHandler mapTileset;
    Tilemap tilemap;
    Camera camera;
    std::vector<Star> stars;
    int starsCollected{0};
    GameState currentState{GameState::Playing};
    Portal portal;
public:

    Engine();
    ~Engine();

    bool Initialize(
        const std::string& title,
        int width,
        int height,
        bool fullscreen,
        int fps);

    void Run();
    void ProcessEvents();
    void Update(float dt);
    void Render();
    void ClearScreen(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a = 255);
    void Shutdown();
};