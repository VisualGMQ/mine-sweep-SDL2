#pragma once

#include "pch.hpp"

class Window final {
public:
    friend class Renderer;

    Window(const std::string& title, int w, int h);
    Window(const Window&) = delete;
    Window(Window&&);
    Window& operator=(const Window&) = delete;

    ~Window();

private:
    SDL_Window* window_;
};