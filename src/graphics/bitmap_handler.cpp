#include "graphics/bitmap_handler.h"
#include <iostream>

BitmapHandler::BitmapHandler(int width, int height) {
    create_bitmap(width, height);
};
BitmapHandler::BitmapHandler(const std::string& filepath)
{
    load_from_file(filepath);
}
BitmapHandler::~BitmapHandler()
{
    delete_bitmap();
}
BitmapHandler::BitmapHandler(BitmapHandler&& other) noexcept : surface(other.surface)
{
    other.surface = nullptr;
}
BitmapHandler& BitmapHandler::operator=(BitmapHandler&& other) noexcept
{
    if (this != &other)
    {
        delete_bitmap();
        surface = other.surface;
        other.surface = nullptr;
    }
    return *this;
}
bool BitmapHandler::create_bitmap(int width, int height) {
    delete_bitmap();
    if (width <= 0 || height <= 0) {
        return false;
    }
    surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    return surface != nullptr;
};
void BitmapHandler::delete_bitmap() {
    if (surface) {
        SDL_DestroySurface(surface);
        surface = nullptr;
    }
}

bool BitmapHandler::load_from_file(const std::string& filepath) {
    delete_bitmap();
    surface = SDL_LoadBMP(filepath.c_str());
    return surface != nullptr;
}

bool BitmapHandler::save_to_file(const std::string& filepath) {
    if (!surface) {
        return false;
    }
    return SDL_SaveBMP(surface, filepath.c_str());
}
bool BitmapHandler::copy_bitmap(const BitmapHandler& source, const SDL_Rect* srcRect, const SDL_Rect* dstRect) {
    if (!surface || !source.surface) {
        return false;
    }
    return SDL_BlitSurface(source.surface, srcRect, surface, dstRect);
}
bool BitmapHandler::LoadFragment(const std::string& filepath, const SDL_Rect& sourceRect) {
    BitmapHandler fullImage(filepath);
    if (!fullImage.isValid()) {
        return false;
    }
    if (!create_bitmap(sourceRect.w, sourceRect.h)) {
        return false;
    }
    SDL_Rect dest = {0,0, sourceRect.w, sourceRect.h};
    return copy_bitmap(fullImage, &sourceRect, &dest);
}

bool BitmapHandler::SaveFragment(const std::string&filepath, const SDL_Rect& area) const{
    if (!surface) {
        return false;
    }
    BitmapHandler cropped(area.w, area.h);
    if (!cropped.isValid()) {
        return false;
    }
    SDL_Rect dest = {0,0, area.w, area.h};
    if (!cropped.copy_bitmap(*this, &area, &dest)) {
        return false;
    }
    return cropped.save_to_file(filepath);
};