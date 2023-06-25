#pragma once
#include "pch.hpp"
#include "window.hpp"

inline auto RendererDestroy = [](SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
};

class Renderer final {
public:
    friend struct Context;

    Renderer(const Window&);

    void SetColor(const SDL_Color&);
    void Clear();
    void Present();
    void DrawRect(const SDL_Rect& rect);
    void FillRect(const SDL_Rect& rect);
    void DrawLine(const SDL_Point& p1, const SDL_Point& p2);
    void DrawTexture(SDL_Texture*, const SDL_Rect&, int x, int y);

private:
    std::unique_ptr<SDL_Renderer, decltype(RendererDestroy)> renderer_;
};