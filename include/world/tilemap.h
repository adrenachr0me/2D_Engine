
#ifndef ENGINE2D_TILEMAP_H
#define ENGINE2D_TILEMAP_H
#pragma once
#include "object_hierarchy.h"
#include <vector>
class BitmapHandler;
class PrimitiveRenderer;
struct Camera;
class Tilemap : public DrawableObject {
private:
    int mapWidth{0};
    int mapHeight{0};
    int tileSize{32};
    const BitmapHandler* tileset{nullptr};
    std::vector<int> tiles;

public:
    Tilemap() = default;
    Tilemap(int width, int height, int tileSize, const BitmapHandler* tilesetBitmap);
    void SetTiles(const std::vector<int>& tiles,int width, int height);
    bool isSolid(int tileX, int tileY) const;
    bool CheckCollision(float worldX, float worldY, float width, float height) const;
    int GetTileSize() const { return tileSize; }
    int GetMapWidth() const { return mapWidth; }
    int GetMapHeight() const { return mapHeight; }
    int GetWorldWidth() const { return mapWidth * tileSize; }
    int GetWorldHeight() const { return mapHeight * tileSize; }
    void Draw(PrimitiveRenderer& renderer) override;
    void Draw(PrimitiveRenderer& renderer, const Camera& camera);
};
#endif //ENGINE2D_TILEMAP_H