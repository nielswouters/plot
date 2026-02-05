#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class Camera
{
public:
    // Default constructor with standard camera settings
    Camera();

    // vfov is top to bottom in degrees;
    Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture = 0., float focus_dist = 1.0);

    Ray get_ray(float s, float t) const;

    vec3 GetOrigin() const { return origin; }
    
    // Input handling
    void UpdateMouseLook(float deltaX, float deltaY, float sensitivity = 0.1f);
    void UpdateMovement(float forward, float right, float up, float deltaTime, float speed = 50.0f);
    void Update(float deltaTime);

private:
    vec3 random_in_unit_disk() const;
    void RecalculateCamera();

    vec3 left_upper_corner;
    vec3 mHorizontal;
    vec3 vertical;
    vec3 origin;
    vec3 u, v, w;
    float lens_radius;
    
    // Camera control
    vec3 position;
    float yaw = 0.0f;      // Rotation around Y-axis (left/right)
    float pitch = 0.0f;    // Rotation around X-axis (up/down)
    
    // Velocity for smooth movement
    vec3 velocity;
    vec3 desired_velocity;
    
    // Camera parameters
    float vfov = 90.0f;
    float aspect = 2.0f;
    float aperture = 0.0f;
    float focus_dist = 1.0f;
    vec3 world_up;
};

#endif
