#include "renderer.hpp"

Renderer::Renderer(const Window& window) {
    renderer_ = SDL_CreateRenderer(window.window_, -1, 0);
}

void Renderer::SetColor(const SDL_Color& c) {
    SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, c.a);
}

Renderer::Renderer(Renderer&& oth) {
    renderer_ = oth.renderer_;
    oth.renderer_ = nullptr;
}

void Renderer::Clear() {
    SDL_RenderClear(renderer_);
}

void Renderer::Present() {
    SDL_RenderPresent(renderer_);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
}

void Renderer::DrawRect(const SDL_Rect& rect) {
    SDL_RenderDrawRect(renderer_, &rect);
}

void Renderer::FillRect(const SDL_Rect& rect) {
    SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::DrawLine(const SDL_Point& p1, const SDL_Point& p2) {
    SDL_RenderDrawLine(renderer_, p1.x, p1.y, p2.x, p2.y);
}

void Renderer::DrawTexture(SDL_Texture* texture, const SDL_Rect& rect, int x, int y) {
    SDL_Rect dst = {x, y, rect.w, rect.h};
    SDL_RenderCopy(renderer_, texture, &rect, &dst);
}