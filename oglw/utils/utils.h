#pragma once

#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

namespace OGLW {

static inline float rad2deg(float _theta) {
    return _theta / M_PI * 180.0f;
}

static inline float deg2rad(float _theta) {
    return _theta * 180.0f / M_PI;
}

static inline float clamp(float _x, float _a, float _b) {
    return _x < _a ? _a : (_x > _b ? _b : _x);
}

bool stringFromPath(const std::string& path, std::string* into);

unsigned char* bytesFromPath(const char* _path, unsigned int* _size);

} // OGLW
