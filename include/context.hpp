#pragma once
#include "window.hpp"
#include "renderer.hpp"
#include "matrix.hpp"

struct Tile {
    enum Type {
        Bomb = -1,
    } type = static_cast<Type>(0);
    bool isVisiable = false;
    bool isFlaged = false;

};

constexpr int TileLen = 32;
constexpr int WindowWidth = TileLen * 20;
constexpr int WindowHeight = TileLen * 20;
constexpr int MineCount = 50;


using Map = Matrix<Tile>;

Map CreateRandomMap(int bombCount, int w, int h);

inline auto TextureDestroy = [](SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
};

struct Context final {
    enum GameState {
        Gaming,
        Win,
        Explode,
    };
    GameState state = Gaming;

    std::unique_ptr<SDL_Texture, decltype(TextureDestroy)> numberImage;
    std::unique_ptr<SDL_Texture, decltype(TextureDestroy)> mineImage;
    std::unique_ptr<SDL_Texture, decltype(TextureDestroy)> flagImage;
    std::unique_ptr<SDL_Texture, decltype(TextureDestroy)> gameoverImage;
    std::unique_ptr<SDL_Texture, decltype(TextureDestroy)> winImage;
    Window window;
    Renderer renderer;
    Map map;

    bool debugMode = false;
    int nakkedCount = 0;
    int mineCount = 0;
    SDL_Event event;
    bool shouldClose = false;

    static void Init() {
        if (!instance_) {
            Window window("mine-sweep", WindowWidth, WindowHeight);
            Renderer renderer(window);
            instance_ = std::unique_ptr<Context>(new Context(
                std::move(window),
                std::move(renderer),
                CreateRandomMap(MineCount, WindowWidth / TileLen, WindowHeight / TileLen),
                MineCount
            ));
        }
    }

    static void Quit() {
        instance_.reset();
    }

    static Context& Inst() {
        SDL_assert(instance_);
        return *instance_;
    }

    void DrawMap();
    void HandleEvent(SDL_Event&);

private:
    static std::unique_ptr<Context> instance_;

    Context(Window&& window, Renderer&& renderer, Map&& map, int mineCount);

    void drawOneTile(int x, int y, const Tile& tile);
    void handleMouseLeftBtnDown(const SDL_MouseButtonEvent&);
    void handleMouseRightBtnDown(const SDL_MouseButtonEvent&);
    void handleKeyDown(const SDL_KeyboardEvent&);
};