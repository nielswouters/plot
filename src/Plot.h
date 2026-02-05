#ifndef _PLOT_H
#define _PLOT_H

#include <stdint.h>

class Surface;
class Hitable;
class Material;
class Camera;

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
    
    Camera *camera;
    float input_forward = 0;   // -1 to 1
    float input_right = 0;     // -1 to 1
    float input_up = 0;        // -1 to 1
};

#endif
