#include <iostream>
#include <stdlib.h>
#include <math.h>

class vec3 
{
public:
    vec3() {} //default constructor
    vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; } // read-only access
    inline float& operator[](int i) { return e[i]; } // read/write access

    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline float length_squared() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
    inline void make_unit_vector() { *this /= length(); }
    inline vec3 unit_vector() const { return *this / length(); }

    inline void integer255_output(std::ostream& out);

    // Friend declarations for operator overloads
    friend std::istream& operator>>(std::istream &is, vec3 &t);
    friend std::ostream& operator<<(std::ostream &os, const vec3 &t);
    friend vec3 operator+(const vec3 &v1, const vec3 &v2);
    friend vec3 operator-(const vec3 &v1, const vec3 &v2);
    friend vec3 operator*(const vec3 &v1, const vec3 &v2);
    friend vec3 operator/(const vec3 &v1, const vec3 &v2);
    friend vec3 operator+(const vec3 &v1, float t);
    friend vec3 operator+(float t, const vec3 &v1);
    friend vec3 operator*(float t, const vec3 &v);
    friend vec3 operator*(const vec3 &v, float t);
    friend vec3 operator/(const vec3 &v, float t);
    friend vec3 sqrt(const vec3 &v);
    friend float dot(const vec3 &v1, const vec3 &v2);
    friend vec3 cross(const vec3 &v1, const vec3 &v2);
private:
    float e[3];
};

// Overloading the input operator for vec3
inline std::istream& operator>>(std::istream &is, vec3 &t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

// Overloading the output operator for vec3
inline std::ostream& operator<<(std::ostream &os, const vec3 &t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline void vec3::integer255_output(std::ostream& out)
{
    *this *= 255.99;
    int e0 = int(e[0]);
    int e1 = int(e[1]);
    int e2 = int(e[2]);
    out << e0 << ' ' << e1 << ' ' << e2 << std::endl;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator+(const vec3 &v, float t)
{
    return vec3(v.e[0] + t, v.e[1] + t, v.e[2] + t);
}

inline vec3 operator+(float t, const vec3 &v)
{
    return vec3(t + v.e[0], t + v.e[1], t + v.e[2]);
}

inline vec3 operator*(float t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, float t)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator/(const vec3 &v, float t)
{
    return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 sqrt(const vec3 &v)
{
    return vec3(sqrt(v.e[0]), sqrt(v.e[1]), sqrt(v.e[2]));
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3& vec3::operator+=(const vec3 &v2)
{
    e[0] += v2.e[0];
    e[1] += v2.e[1];
    e[2] += v2.e[2];
    return *this; // we return a MODIFIABLE reference to ourselves. We do this so we can write things like: (a += b) += c;
}

inline vec3& vec3::operator-=(const vec3 &v2)
{
    e[0] -= v2.e[0];
    e[1] -= v2.e[1];
    e[2] -= v2.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3 &v2)
{
    e[0] *= v2.e[0];
    e[1] *= v2.e[1];
    e[2] *= v2.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3 &v2)
{
    e[0] /= v2.e[0];
    e[1] /= v2.e[1];
    e[2] /= v2.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(const float t)
{
    float k = 1.0f / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}