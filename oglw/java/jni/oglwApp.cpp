#include "oglwApp.h"
#include <cmath>

namespace OGLW {

void OGLWApp::init() { }

void OGLWApp::update(float _dt) { }

void OGLWApp::render(float _dt) { 
    static float t = 0.f;
    t += 0.1f;
    glClearColor(sin(t) * 0.5f + 0.5f, cos(t) * 0.5f + 0.5f, 0.f, 1.f);
}

}
