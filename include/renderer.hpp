#pragma once
#include "pch.hpp"
#include "window.hpp"

class Renderer final {
public:
    friend struct Context;

    Renderer(const Window&);
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&);
    Renderer& operator=(const Renderer&) = delete;

    void SetColor(const SDL_Color&);
    void Clear();
    void Present();
    void DrawRect(const SDL_Rect& rect);
    void FillRect(const SDL_Rect& rect);
    void DrawLine(const SDL_Point& p1, const SDL_Point& p2);
    void DrawTexture(SDL_Texture*, const SDL_Rect&, int x, int y);

private:
    SDL_Renderer* renderer_;
};