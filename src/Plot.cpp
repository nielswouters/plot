#include "Plot.h"

#include "Surface.h"

#include <stdio.h>

Plot::Plot()
{

}

Plot::~Plot()
{

}

void Plot::MouseMove(uint32_t _X, uint32_t _Y)
{
    printf("Mouse Move: %u %u\n", _X, _Y);
}

void Plot::MouseButton(uint32_t _Button, bool _Down)
{
    printf("Mouse Button: %u %s\n", _Button, _Down ? "Down" : "Up");
}

void Plot::KeyboardButton(uint32_t _Button, bool _Down)
{
    printf("Keyboard Button: %u %s\n", _Button, _Down ? "Down" : "Up");
}

void Plot::Update(float _DeltaTime)
{

}

void Plot::Draw(Surface* _Screen)
{
    _Screen->Clear();

    static uint32_t offset = 0;
    offset++;

    uint32_t width = _Screen->GetWidth();
    uint32_t height = _Screen->GetHeight();

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            uint32_t r = (x + offset) % 255;
            uint32_t g = y % 255;
            uint32_t b = 0;

            uint32_t c = r << 16 | g << 8 | b;

            _Screen->Plot(x, y, c);
        }
    }
}
