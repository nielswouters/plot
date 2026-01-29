#include "Surface.h"
#include "Plot.h"

#include <SDL3/SDL.h>

#include <assert.h>

#include <chrono>

#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

    SDL_Window* window = window = SDL_CreateWindow("Plot", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
    {
        PrintSDLError("SDL could not create a window");
    }
    else
    {
        SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

        Plot plot;

        bool run = true;
        while (run)
        {
            if (!SDL_UpdateWindowSurface(window))
            {
                PrintSDLError("SDL_UpdateWindowSurface failed");
                run = false;
            }
            else
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
                        screenSurface = SDL_GetWindowSurface(window);
                        break;

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

                float deltaTime = DeltaTime();
                plot.Update(deltaTime);

                assert(screenSurface->format == SDL_PIXELFORMAT_XRGB8888);
                Surface surface((uint32_t*)screenSurface->pixels, screenSurface->w, screenSurface->h, screenSurface->pitch / 4);

                plot.Draw(&surface);
            }
        }

        SDL_DestroyWindow(window);
    }

    SDL_Quit();

    return 0;
}
