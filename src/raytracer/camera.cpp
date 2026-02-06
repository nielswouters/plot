#include "camera.h"

#include "random.h"
#include <cmath>

#ifndef M_PI
    #define M_PI  3.14159265359
#endif

Camera::Camera()
    : Camera(vec3(0., 0., 0.), vec3(0., 0., -1.), vec3(0., 1., 0.), 90., 2.0, 0., 1.0)
{

}

Camera::Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
    : position(lookfrom), vfov(vfov), aspect(aspect), 
      aperture(aperture), focus_dist(focus_dist), world_up(vup),
      velocity(0., 0., 0.), desired_velocity(0., 0., 0.)
{
    // Calculate initial yaw/pitch from lookfrom->lookat direction
    vec3 direction = (lookat - lookfrom).unit_vector();
    yaw = atan2(direction.x(), -direction.z()) * 180.0f / M_PI;
    pitch = asin(direction.y()) * 180.0f / M_PI;
    
    RecalculateCamera();
}

void Camera::RecalculateCamera()
{
    // Convert yaw/pitch to direction vector
    float yaw_rad = yaw * M_PI / 180.0f;
    float pitch_rad = pitch * M_PI / 180.0f;
    
    vec3 direction(
        sin(yaw_rad) * cos(pitch_rad),
        sin(pitch_rad),
        -cos(yaw_rad) * cos(pitch_rad)
    );
    
    lens_radius = aperture / 2;
    float theta = vfov * M_PI / 180.;
    float half_height = tan(theta / 2);
    float half_width = aspect * half_height;

    w = -direction;  // w points opposite to camera direction
    u = cross(world_up, w).unit_vector();
    v = cross(w, u);

    origin = position;
    left_upper_corner = origin - half_width * focus_dist * u + half_height * focus_dist * v - focus_dist * w;
    mHorizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
}

void Camera::UpdateMouseLook(float deltaX, float deltaY, float sensitivity)
{
    // Mouse Y controls pitch (looking up/down)
    pitch -= deltaY * sensitivity;
    
    // Clamp pitch to prevent flipping
    if (pitch > 40.0f) pitch = 40.0f;
    if (pitch < -40.0f) pitch = -40.0f;
    
    // Mouse X controls yaw (looking left/right)
    yaw += deltaX * sensitivity;
    
    // Normalize yaw to 0-360
    if (yaw > 360.0f) yaw -= 360.0f;
    if (yaw < 0.0f) yaw += 360.0f;
}

void Camera::UpdateMovement(float forward, float right, float up, 
                           float deltaTime, float speed)
{
    // Calculate movement vectors based on current camera orientation
    float yaw_rad = yaw * M_PI / 180.0f;
    
    // Forward vector (based on yaw only, ignore pitch)
    vec3 forward_dir(sin(yaw_rad), 0, -cos(yaw_rad));
    
    // Right vector
    vec3 right_dir(cos(yaw_rad), 0, sin(yaw_rad));
    
    // Build desired velocity
    desired_velocity = (forward_dir * forward + right_dir * right + vec3(0, 1, 0) * up) * speed;
}

void Camera::Update(float deltaTime)
{
    // Smooth acceleration/deceleration (lerp velocity towards desired)
    float acceleration = 15.0f;  // How quickly we reach target speed
    
    if (deltaTime > 0) {
        velocity[0] += (desired_velocity.x() - velocity.x()) * acceleration * deltaTime;
        velocity[1] += (desired_velocity.y() - velocity.y()) * acceleration * deltaTime;
        velocity[2] += (desired_velocity.z() - velocity.z()) * acceleration * deltaTime;
    }
    
    // Apply velocity to position
    position = position + velocity * deltaTime;
    
    // Recalculate camera matrices
    RecalculateCamera();
}

Ray Camera::get_ray(float s, float t) const
{
    if (lens_radius > 0) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset, left_upper_corner + s * mHorizontal - t * vertical - origin - offset);
    }
    return Ray(origin, left_upper_corner + s * mHorizontal - t * vertical - origin);
};

vec3 Camera::random_in_unit_disk() const
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(random_float(), random_float(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}
