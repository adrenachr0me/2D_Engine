
#ifndef ENGINE2D_TILEMAP_H
#define ENGINE2D_TILEMAP_H
#pragma once
#include "object_hierarchy.h"
#include <string>
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
    int firstSolidTile{-1};

public:
    Tilemap() = default;
    Tilemap(int width, int height, int tileSize, const BitmapHandler* tilesetBitmap);
    void SetTiles(const std::vector<int>& tiles,int width, int height);
    bool LoadFromFile(const std::string& path, const BitmapHandler* tilesetBitmap);
    bool isSolid(int tileX, int tileY) const;
    bool CheckCollision(float worldX, float worldY, float width, float height) const;
    int GetTileSize() const { return tileSize; }
    int GetMapWidth() const { return mapWidth; }
    int GetMapHeight() const { return mapHeight; }
    int GetWorldWidth() const { return mapWidth * tileSize; }
    int GetWorldHeight() const { return mapHeight * tileSize; }
    void Draw(PrimitiveRenderer& renderer) override;
    void Draw(PrimitiveRenderer& renderer, const Camera& camera);
    bool IsTileSolid(int tileID) const {
        if (firstSolidTile >= 0)
            return tileID >= firstSolidTile;
        return tileID == 1090 || tileID == 1;
    }

    bool IsSolidAtTile(int tx, int ty) const {
        if (tx < 0 || tx >= mapWidth || ty < 0 || ty >= mapHeight) {
            return true;
        }
        int id = tiles[ty * mapWidth + tx];
        return IsTileSolid(id);
    }
};
#endif //ENGINE2D_TILEMAP_H
