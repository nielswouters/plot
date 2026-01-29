#ifndef _SURFACE_H
#define _SURFACE_H

#include <stdint.h>

class Surface
{
public:
    Surface(uint32_t* _Pixels, uint32_t _Width, uint32_t _Height, uint32_t _Pitch);
    ~Surface();

    void Clear();
    void Clear(uint32_t _Color);
    void Plot(uint32_t _X, uint32_t _Y, uint32_t _Color);
    void Print(uint32_t _X, uint32_t _Y, const char* _Str, uint32_t _Color);

    uint32_t GetWidth() { return mWidth; }
    uint32_t GetHeight() { return mHeight; }

private:
    uint32_t* mPixels;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mPitch;
};

#endif
