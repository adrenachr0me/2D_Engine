
#ifndef ENGINE2D_BITMAP_HANDLER_H
#define ENGINE2D_BITMAP_HANDLER_H
#pragma once

#include <SDL3/SDL.h>
#include <string>

class BitmapHandler {
private:
    SDL_Surface* surface{nullptr};
public:
    BitmapHandler() = default;
    BitmapHandler(int width, int height);
    explicit BitmapHandler(const std::string& filepath);

    ~BitmapHandler();

    BitmapHandler(const BitmapHandler&) = delete;
    BitmapHandler& operator=(const BitmapHandler&) = delete;
    BitmapHandler(BitmapHandler&& other) noexcept;
    BitmapHandler& operator=(BitmapHandler&& other) noexcept;
    bool create_bitmap(int width,int height);
    void delete_bitmap();
    bool load_from_file(const std::string& filepath);
    bool save_to_file(const std::string& filepath);
    bool LoadFragment(const std::string& filepath, const SDL_Rect& sourceRect);
    bool SaveFragment(const std::string& filepath, const SDL_Rect& area) const;
    bool copy_bitmap(const BitmapHandler& source, const SDL_Rect* srcRect = nullptr, const SDL_Rect* dstRect = nullptr);
    SDL_Surface* GetRawSurface() const {return surface;};
    int GetWidth() const {return surface ? surface->w : 0;}
    int GetHeight() const {return surface ? surface->h : 0;}
    bool isValid() const {return surface != nullptr;}
};


#endif //ENGINE2D_BITMAP_HANDLER_H