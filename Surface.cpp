#include "Surface.h"

#include <string.h>

#include <vector>

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

void Surface::Print(uint32_t _X, uint32_t _Y, const char* _Str, uint32_t _Color)
{
    static std::vector<uint32_t> font;
    if (font.size() == 0)
    {
        font.resize(256);

        font['a']  = 0b0111010001111111000110001;
        font['b']  = 0b1111010001111101000111110;
        font['c']  = 0b0111110000100001000001111;
        font['d']  = 0b1111010001100011000111110;
        font['e']  = 0b1111110000111101000011111;
        font['f']  = 0b1111110000111001000010000;
        font['g']  = 0b0111110000101111000101110;
        font['h']  = 0b1000110001111111000110001;
        font['i']  = 0b0010000100001000010000100;
        font['j']  = 0b0001000010000100001011100;
        font['k']  = 0b1001010100110001010010010;
        font['l']  = 0b1000010000100001000011111;
        font['m']  = 0b1101010101101011000110001;
        font['n']  = 0b1000111001101011001110001;
        font['o']  = 0b0111010001100011000101110;
        font['p']  = 0b1111010001111101000010000;
        font['q']  = 0b0111010001100011001101111;
        font['r']  = 0b1111010001111101010010010;
        font['s']  = 0b0111110000011100000111110;
        font['t']  = 0b1111100100001000010000100;
        font['u']  = 0b1000110001100011000101111;
        font['v']  = 0b1000110001010100101000100;
        font['w']  = 0b1000110001101011010101010;
        font['x']  = 0b1000101010001000101010001;
        font['y']  = 0b1000110001011110000101110;
        font['z']  = 0b1111100010001000100011111;
        font['0']  = 0b0111010011101011100101110;
        font['1']  = 0b0010001100001000010001110;
        font['2']  = 0b0111010001001100100011111;
        font['3']  = 0b1111000001001100000111110;
        font['4']  = 0b1000010010111110001000010;
        font['5']  = 0b1111110000111100000111110;
        font['6']  = 0b0111110000111101000101110;
        font['7']  = 0b1111100001000100010000100;
        font['8']  = 0b0111010001011101000101110;
        font['9']  = 0b0111010001011110000101110;
        font['!']  = 0b0010000100001000000000100;
        font['?']  = 0b0111000001000100000000100;
        font[':']  = 0b0000000000001000000000100;
        font['=']  = 0b0000000000011100000001110;
        font[',']  = 0b0000000000000000001000100;
        font['.']  = 0b0000000000000000000000100;
        font['-']  = 0b0000000000011100000000000;
        font['(']  = 0b0001000100001000010000010;
        font[')']  = 0b0010000010000100001000100;
        font[' ']  = 0b0000000000000000000000000;
        font['#']  = 0b1111111111111111111111111;
        font['\''] = 0b0010000100000000000000000;
        font['*']  = 0b1010101110111110111010101;
        font['/']  = 0b0000100010001000100010000;
        font['"']  = 0b0111010001111111000110001;

        for (char c = 'A'; c < 'Z'; c++)
        {
            font[c] = font['a' + c - 'A'];
        }
    }

    uint32_t strLength = (uint32_t)strlen(_Str);
    for (uint32_t i = 0; i < strLength; i++)
    {
        const char c = _Str[i];
        uint32_t letter = font[c];

        for (uint32_t y = 0; y < 10; y++)
        {
            for (uint32_t x = 0; x < 10; x++)
            {
                uint32_t px = x >> 1;
                uint32_t py = y >> 1;
                uint32_t bitIndex = py * 5 + px;

                if (letter & (1u << (24 - bitIndex)))
                {
                    uint32_t pixelIdx = (_Y + y) * mPitch + _X + x + i * 12;
                    mPixels[pixelIdx] = _Color;
                }
            }
        }
    }
}
