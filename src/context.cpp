#include "context.hpp"


constexpr SDL_Color NormalTileColor = {150, 150, 150, 255};
constexpr SDL_Color HoverTileColor = {200, 200, 200, 255};
constexpr SDL_Color BorderTileColor = {0, 0, 0, 255};
constexpr SDL_Color NakedTileColor = {50, 50, 50, 255};
constexpr SDL_Color KeyColor = {118, 66, 138, 255};

std::unique_ptr<Context> Context::instance_ = nullptr;

Map CreateRandomMap(int bombCount, int w, int h) {
    SDL_assert(bombCount < w * h);

    std::random_device d;
    std::mt19937 gen(d());
    std::uniform_int_distribution dist1(0, w - 1);
    std::uniform_int_distribution dist2(0, h - 1);

    Map map(w, h);

    constexpr int MaxBombSetupCount = 100;

    // generate all mines
    while (bombCount > 0) {
        bool setupBomb = false;
        int setupCount = MaxBombSetupCount;
        while (!setupBomb && setupCount > 0) {
            int x = dist1(gen);
            int y = dist2(gen);
            auto& tile = map.Get(x, y);
            if (tile.type != Tile::Bomb) {
                tile.type = Tile::Bomb;
                setupBomb = true;
            } else {
                setupCount --;
            }
        }

        if (setupCount == 0) {
            for (int i = 0; i < map.MaxSize(); i++) {
                if (map.GetByIndex(i).type != Tile::Bomb) {
                    map.GetByIndex(i).type = Tile::Bomb;
                    break;
                }
            }
        }

        bombCount --;
    }

    // generate nearly mine number
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            auto& tile = map.Get(x, y);
            if (tile.type == Tile::Bomb) {
                continue;
            }

            int mineCount = 0;
            for (int nx = -1; nx <= 1; nx ++) {
                for (int ny = -1; ny <= 1; ny++) {
                    int detectX = x + nx;
                    int detectY = y + ny;
                    if (map.IsIn(detectX, detectY)) {
                        if (map.Get(detectX, detectY).type == Tile::Bomb) {
                            mineCount ++;
                        }
                    }
                }
            }

            tile.type = static_cast<Tile::Type>(mineCount);
        }
    }

    return map;
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& bmpFilename, const SDL_Color& keycolor) {
    SDL_Surface* surface = SDL_LoadBMP(bmpFilename.c_str());
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, keycolor.r, keycolor.g, keycolor.b));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Context::Context(Window&& window, Renderer&& renderer, Map&& map, int mineCount)
    : numberImage(loadTexture(renderer.renderer_, "resources/font.bmp", KeyColor), TextureDestroy),
      mineImage(loadTexture(renderer.renderer_,"resources/mine.bmp", KeyColor), TextureDestroy),
      flagImage(loadTexture(renderer.renderer_,"resources/flag.bmp", KeyColor), TextureDestroy),
      gameoverImage(loadTexture(renderer.renderer_,"resources/gameover.bmp", KeyColor), TextureDestroy),
      winImage(loadTexture(renderer.renderer_,"resources/win.bmp", KeyColor), TextureDestroy),
      window(std::move(window)),
      renderer(std::move(renderer)),
      map(std::move(map)),
      mineCount(mineCount) {}

void Context::DrawMap() {
    for (int y = 0; y < map.Height(); y++) {
        for (int x = 0; x < map.Width(); x++) {
            const auto& tile = map.Get(x, y);
            drawOneTile(x, y, tile);
        }
    }

    if (state == GameState::Explode) {
        renderer.DrawTexture(gameoverImage.get(), SDL_Rect{0, 0, 256, 256},
                             (WindowWidth - 256) / 2,
                             (WindowHeight - 256) / 2);
    }
    if (state == GameState::Win) {
        renderer.DrawTexture(winImage.get(), SDL_Rect{0, 0, 256, 256},
                             (WindowWidth - 256) / 2,
                             (WindowHeight - 256) / 2);
    }
}

void Context::drawOneTile(int x, int y, const Tile& tile) {
    int tileX = x * TileLen;
    int tileY = y * TileLen;
    SDL_Rect rect = {tileX, tileY, TileLen, TileLen};
    SDL_Point mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    if (SDL_PointInRect(&mousePos, &rect)) {
        renderer.SetColor(HoverTileColor);
    } else {
        renderer.SetColor(NormalTileColor);
    }
    if (tile.isVisiable && static_cast<int>(tile.type) >= 0) {
        renderer.SetColor(NakedTileColor);
    }
    renderer.FillRect(rect);
    renderer.SetColor(BorderTileColor);
    renderer.DrawRect(rect);

    if (tile.isVisiable) {
        if (tile.type == Tile::Bomb) {
            renderer.DrawTexture(mineImage.get(), SDL_Rect{0, 0, 32, 32}, tileX, tileY);
        } else {
            int mineCount = tile.type;
            if (mineCount > 0) {
                renderer.DrawTexture(
                    numberImage.get(),
                    SDL_Rect{309 / 8 * (mineCount - 1), 0, 32, 32},
                    tileX, tileY);
            }
        }
    } else {
        if (tile.isFlaged) {
            renderer.DrawTexture(flagImage.get(), SDL_Rect{0, 0, 32, 32}, tileX, tileY);
        }
    }

    if (debugMode) {
        if (tile.type == Tile::Bomb) {
            renderer.DrawTexture(mineImage.get(), SDL_Rect{0, 0, 32, 32}, tileX, tileY);
        } else {
            int mineCount = tile.type;
            if (mineCount > 0) {
                renderer.DrawTexture(
                    numberImage.get(),
                    SDL_Rect{309 / 8 * (mineCount - 1), 0, 32, 32},
                    tileX, tileY);
            }
        }
    }
}

void Context::HandleEvent(SDL_Event& e) {
    if (!state == GameState::Gaming) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            map = CreateRandomMap(MineCount, WindowWidth / TileLen, WindowHeight / TileLen);
            mineCount = MineCount;
            nakkedCount = 0;
            state = GameState::Gaming;
        }
        return;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            handleMouseLeftBtnDown(e.button);
        }
        if (e.button.button == SDL_BUTTON_RIGHT) {
            handleMouseRightBtnDown(e.button);
        }
    }
    if (e.type == SDL_KEYDOWN) {
        handleKeyDown(e.key);
    }
}

inline SDL_Point calcTileCoord(int mouseX, int mouseY) {
    return {mouseX / TileLen, mouseY / TileLen};
}

void floodFill(Context& ctx, Map& map, int x, int y) {
    if (!map.IsIn(x, y)) {
        return;
    }

    auto& tile = map.Get(x, y);
    if (!tile.isVisiable && !tile.isFlaged && tile.type != Tile::Bomb) {
        tile.isVisiable = true;
        ctx.nakkedCount ++;
        if (static_cast<int>(tile.type) == 0) {
            floodFill(ctx, map, x - 1, y);
            floodFill(ctx, map, x + 1, y);
            floodFill(ctx, map, x, y - 1);
            floodFill(ctx, map, x, y + 1);
        }
    }
}

void Context::handleMouseLeftBtnDown(const SDL_MouseButtonEvent& e) {
    auto tileCoord = calcTileCoord(e.x, e.y);
    if (!map.IsIn(tileCoord.x, tileCoord.y)) {
        return;
    }

    auto& tile = map.Get(tileCoord.x, tileCoord.y);
    if (tile.isVisiable || tile.isFlaged) {
        return;
    }

    if (!tile.isVisiable && tile.type == Tile::Bomb) {
        state = GameState::Explode;
        for (int i = 0; i < map.MaxSize(); i++) {
            auto& tile = map.GetByIndex(i);
            tile.isVisiable = true;
            tile.isFlaged = false;
        }
        return;
    }

    floodFill(*this, map, tileCoord.x, tileCoord.y);

    if (nakkedCount == map.MaxSize() - mineCount) {
        state = GameState::Win;
    }
}

void Context::handleMouseRightBtnDown(const SDL_MouseButtonEvent& e) {
    auto tileCoord = calcTileCoord(e.x, e.y);
    if (!map.IsIn(tileCoord.x, tileCoord.y)) {
        return;
    }

    auto& tile = map.Get(tileCoord.x, tileCoord.y);

    if (!tile.isVisiable) {
        tile.isFlaged = !tile.isFlaged;
    }

}

void Context::handleKeyDown(const SDL_KeyboardEvent& e) {
    if (e.keysym.scancode == SDL_SCANCODE_G) {
        debugMode = !debugMode;
    }
}