#ifndef RANDOMH
#define RANDOMH

#include <random>

// Thread-local random number generator for efficient, thread-safe random number generation
inline float random_float() {
    thread_local std::mt19937 generator(std::random_device{}());
    thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return distribution(generator);
}

inline float random_float(float min, float max) {
    return min + (max - min) * random_float();
}

#endif
