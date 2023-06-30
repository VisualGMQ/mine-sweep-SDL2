#include <iostream>
#include "context.hpp"

void Run() {
    auto& ctx = Context::Inst();
    auto& event = ctx.event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            ctx.shouldClose = true;
        }
        ctx.HandleEvent(event);
    }

    auto& renderer = ctx.renderer;
    renderer.SetColor(SDL_Color{200, 200, 200, 255});
    renderer.Clear();

    ctx.DrawMap();

    renderer.Present();
    SDL_Delay(30);
}

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);

    Context::Init();
    auto& ctx = Context::Inst();
    auto& renderer = ctx.renderer;

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
    emscripten_set_main_loop(Run, 0, 1);
#else
    while(!ctx.shouldClose) {
        Run();
    }
#endif

    Context::Quit();
    SDL_Quit();

    return 0;
}