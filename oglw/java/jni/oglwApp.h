#pragma once

#include "oglw.h"
#include <memory>

namespace OGLW {

template <class T>
using uptr = std::unique_ptr<T>;

class OGLWApp : public App {
    public:
        OGLWApp(float _width, float _height) : App("OGLW::OGLWApp", _width, _height) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;
};

}
