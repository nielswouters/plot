#ifndef _PLOT_H
#define _PLOT_H

#include <stdint.h>

class Surface;
class Hitable;
class Material;

class Plot
{
public:
    Plot();
    ~Plot();

    void MouseMove(uint32_t _X, uint32_t _Y);
    void MouseButton(uint32_t _Button, bool _Down);
    void KeyboardButton(uint32_t _Button, bool _Down);
    void Update(float _DeltaTime);
    void Draw(Surface* _Screen);
    int mFPS;

private:
    Hitable *list[3];
    Hitable *world;
    Material *materials[3];
    bool initialized;
};

#endif
