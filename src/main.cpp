#include "Surface.h"
#include "Plot.h"

#include <SDL3/SDL.h>

#include <assert.h>

#include <chrono>

#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const SDL_PixelFormat BACK_BUFFER_FORMAT = SDL_PIXELFORMAT_ARGB8888;

static void PrintSDLError(const char* _Message)
{
    printf("%s. SDL_Error: %s\n", _Message, SDL_GetError());
}

float DeltaTime()
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

    SDL_Surface* screen = SDL_GetWindowSurface(window);
    if (!screen)
    {
        PrintSDLError("SDL_GetWindowSurface failed");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Surface* backbuffer = SDL_CreateSurface(screen->w, screen->h, BACK_BUFFER_FORMAT);
    if (!backbuffer)
    {
        PrintSDLError("SDL_CreateSurface(backbuffer) failed");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_SetSurfaceBlendMode(backbuffer, SDL_BLENDMODE_NONE);

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
                screen = SDL_GetWindowSurface(window);
                if (!screen)
                {
                    PrintSDLError("SDL_GetWindowSurface failed after resize");
                    run = false;
                    break;
                }

                SDL_DestroySurface(backbuffer);
                backbuffer = SDL_CreateSurface(screen->w, screen->h, BACK_BUFFER_FORMAT);
                if (!backbuffer)
                {
                    PrintSDLError("SDL_CreateSurface(backbuffer) failed after resize");
                    run = false;
                    break;
                }

                SDL_SetSurfaceBlendMode(backbuffer, SDL_BLENDMODE_NONE);
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

        if (!SDL_LockSurface(backbuffer))
        {
            PrintSDLError("SDL_LockSurface failed");
            run = false;
            break;
        }

        Surface surface((uint32_t*)backbuffer->pixels,
            backbuffer->w, backbuffer->h,
            backbuffer->pitch / 4);
        plot.Draw(&surface);

        SDL_UnlockSurface(backbuffer);

        if (!SDL_BlitSurface(backbuffer, NULL, screen, NULL))
        {
            PrintSDLError("SDL_BlitSurface failed");
            run = false;
            break;
        }

        if (!SDL_UpdateWindowSurface(window))
        {
            PrintSDLError("SDL_UpdateWindowSurface failed");
            run = false;
            break;
        }
    }

    SDL_DestroySurface(backbuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
