#include "world/tilemap.h"
#include "graphics/bitmap_handler.h"
#include "graphics/primitiverenderer.h"
#include "graphics/camera.h"

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

bool Tilemap::isSolid(int tileX, int tileY) const{
    if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight)
    {
        return true;
    }
    int tileID = tiles[tileY * mapWidth + tileX];
    return tileID == 1;
}

bool Tilemap::CheckCollision(float worldX, float worldY, float width, float height) const{
    int startCol = static_cast<int>(worldX) / tileSize;
    int endCol   = static_cast<int>(worldX + width) / tileSize;
    int startRow = static_cast<int>(worldY) / tileSize;
    int endRow   = static_cast<int>(worldY + height) / tileSize;
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