#include "world/tilemap.h"
#include "graphics/bitmap_handler.h"
#include "graphics/primitiverenderer.h"
#include "graphics/camera.h"
#include <cmath>
#include <fstream>
#include <limits>
#include <utility>

Tilemap::Tilemap(int width, int height, int tileSize, const BitmapHandler* tilesetBitmap)
    : mapWidth(width), mapHeight(height), tileSize(tileSize), tileset(tilesetBitmap)
{
    tiles.resize(width * height, 0);
}
void Tilemap::SetTiles(const std::vector<int>& new_tiles, int width, int height) {
    mapWidth = width;
    mapHeight = height;
    tiles = new_tiles;
}

bool Tilemap::LoadFromFile(const std::string& path, const BitmapHandler* tilesetBitmap) {
    if (!tilesetBitmap || !tilesetBitmap->isValid())
        return false;

    std::ifstream file(path);
    int width, height, size, solidStart;
    if (!(file >> width >> height >> size >> solidStart))
        return false;
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096 ||
        static_cast<long long>(width) * height > 4194304 || size <= 0 ||
        size > tilesetBitmap->GetWidth() || size > tilesetBitmap->GetHeight() ||
        tilesetBitmap->GetWidth() % size != 0 || tilesetBitmap->GetHeight() % size != 0 ||
        width > std::numeric_limits<int>::max() / size ||
        height > std::numeric_limits<int>::max() / size)
        return false;

    const long long atlasTileCount = static_cast<long long>(tilesetBitmap->GetWidth() / size) *
                                    (tilesetBitmap->GetHeight() / size);
    if (atlasTileCount > std::numeric_limits<int>::max() ||
        solidStart < 0 || solidStart > atlasTileCount)
        return false;

    std::vector<int> loadedTiles(width * height);
    for (int& id : loadedTiles) {
        if (!(file >> id) || id < 0 || id >= atlasTileCount)
            return false;
    }
    file >> std::ws;
    if (!file.eof())
        return false;

    mapWidth = width;
    mapHeight = height;
    tileSize = size;
    firstSolidTile = solidStart;
    tileset = tilesetBitmap;
    tiles = std::move(loadedTiles);
    return true;
}

bool Tilemap::isSolid(int tileX, int tileY) const{
    return IsSolidAtTile(tileX, tileY);
}

bool Tilemap::CheckCollision(float worldX, float worldY, float width, float height) const{
    if (!std::isfinite(worldX) || !std::isfinite(worldY) ||
        !std::isfinite(width) || !std::isfinite(height) ||
        width <= 0.0f || height <= 0.0f || tileSize <= 0 ||
        worldX < 0.0f || worldY < 0.0f ||
        worldX + width > GetWorldWidth() || worldY + height > GetWorldHeight())
        return true;
    int startCol = static_cast<int>(std::floor(worldX / tileSize));
    int endCol   = static_cast<int>(std::floor((worldX + width - 0.01f) / tileSize));
    int startRow = static_cast<int>(std::floor(worldY / tileSize));
    int endRow   = static_cast<int>(std::floor((worldY + height - 0.01f) / tileSize));
    for (int row = startRow; row <= endRow; ++row)
    {
        for (int col = startCol; col <= endCol; ++col)
        {
            if (isSolid(col, row))
            {
                return true;
            }
        }
    }
    return false;
}

void Tilemap::Draw(PrimitiveRenderer& renderer) {
    Camera cam(1920,1080);
    Draw(renderer, cam);
}
void Tilemap::Draw(PrimitiveRenderer& renderer, const Camera& camera)
{
    if (!tileset || !tileset->isValid())
        return;

    int tilesPerRow = tileset->GetWidth() / tileSize;
    if (tilesPerRow <= 0) return;

    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            int tileID = tiles[y * mapWidth + x];
            if (tileID < 0) continue;

            SDL_Rect srcRect;
            srcRect.x = (tileID % tilesPerRow) * tileSize;
            srcRect.y = (tileID / tilesPerRow) * tileSize;
            srcRect.w = tileSize;
            srcRect.h = tileSize;
            float worldX = static_cast<float>(x * tileSize);
            float worldY = static_cast<float>(y * tileSize);
            float screenX = (x * tileSize) - camera.GetX();
            float screenY = (y * tileSize) - camera.GetY();
            if (screenX + tileSize < 0 || screenX > camera.GetViewportWidth() ||
                screenY + tileSize < 0 || screenY > camera.GetViewportHeight())
            {
                continue;
            }
            renderer.DrawBitmapFragment(*tileset, srcRect, screenX, screenY);
        }
    }
}
