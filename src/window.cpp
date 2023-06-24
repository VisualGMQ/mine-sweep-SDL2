#include "window.hpp"

Window::Window(const std::string& title, int w, int h) {
  window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
}

Window::Window(Window&& oth) {
    window_ = oth.window_;
    oth.window_ = nullptr;
}

Window::~Window() {
    SDL_DestroyWindow(window_);
}