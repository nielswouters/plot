#include "Surface.h"
#include "Plot.h"

#include <SDL3/SDL.h>

#include <assert.h>

#include <chrono>

#include <iostream>

const bool VSYNC = true;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const SDL_PixelFormat BACK_BUFFER_FORMAT = SDL_PIXELFORMAT_ARGB8888;

static void PrintSDLError(const char* Message)
{
    const char* err = SDL_GetError();
    if (err && err[0] != '\0')
        printf("%s. SDL_Error: %s\n", Message, err);
}

static float DeltaTime()
{
    using clock = std::chrono::steady_clock;

    static auto lastTime = clock::now();
    auto currentTime = clock::now();

    std::chrono::duration<float, std::milli> delta = currentTime - lastTime;
    lastTime = currentTime;

    return delta.count();
}

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        PrintSDLError("SDL could not be initialized");
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Plot", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
    {
        PrintSDLError("SDL could not create a window");
        SDL_Quit();
        return -1;
    }

    int screenWidth;
    int screenHeight;
    SDL_GetWindowSizeInPixels(window, &screenWidth, &screenHeight);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        PrintSDLError("SDL_CreateRenderer failed");
        return -1;
    }

    if (!SDL_SetRenderVSync(renderer, VSYNC))
    {
        PrintSDLError("SDL_SetRenderVSync failed");
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        BACK_BUFFER_FORMAT,
        SDL_TEXTUREACCESS_STREAMING,
        screenWidth,
        screenHeight
    );
    if (!texture)
    {
        PrintSDLError("SDL_CreateTexture failed");
        return -1;
    }

    Plot plot;
    bool run = true;

    while (run)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                run = false;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            {
                SDL_GetWindowSizeInPixels(window, &screenWidth, &screenHeight);

                if (texture)
                {
                    SDL_DestroyTexture(texture);
                    texture = nullptr;
                }

                texture = SDL_CreateTexture(renderer, BACK_BUFFER_FORMAT, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
                if (!texture)
                {
                    PrintSDLError("SDL_CreateTexture failed");
                    return -1;
                }
            } break;

            case SDL_EVENT_MOUSE_MOTION:
                plot.MouseMove((uint32_t)e.motion.x, (uint32_t)e.motion.y);
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                plot.MouseButton(e.button.button, e.button.down);
                break;

            case SDL_EVENT_KEY_UP:
            case SDL_EVENT_KEY_DOWN:
                plot.KeyboardButton(e.key.key, e.key.down);
                break;
            }
        }

        if (!run) break;

        float deltaTime = DeltaTime();
        plot.Update(deltaTime);

        void* pixels;
        int pitch;
        if (!SDL_LockTexture(texture, nullptr, &pixels, &pitch))
        {
            PrintSDLError("SDL_LockTexture failed");
            run = false;
            break;
        }

        Surface surface((uint32_t*)pixels, screenWidth, screenHeight, pitch / 4);
        plot.Draw(&surface);

        SDL_UnlockTexture(texture);

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
