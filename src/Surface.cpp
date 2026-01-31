#include "Surface.h"

#include <string.h>

Surface::Surface(uint32_t* _Pixels, uint32_t _Width, uint32_t _Height, uint32_t _Pitch)
    : mPixels(_Pixels)
    , mWidth(_Width)
    , mHeight(_Height)
    , mPitch(_Pitch)
{

}

Surface::~Surface()
{

}

void Surface::Clear()
{
    uint32_t sizeInBytes = mHeight * mPitch * 4;
    memset(mPixels, 0x00, sizeInBytes);
}

void Surface::Clear(uint32_t _Color)
{
    for (uint32_t y = 0; y < mHeight; y++)
    {
        for (uint32_t x = 0; x < mWidth; x++)
        {
            Plot(x, y, _Color);
        }
    }
}

void Surface::Plot(uint32_t _X, uint32_t _Y, uint32_t _Color)
{
    uint32_t pixelIdx = _Y * mPitch + _X;
    mPixels[pixelIdx] = _Color;
}
