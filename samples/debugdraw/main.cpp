#include "oglw.h"

using namespace OGLW;

// ------------------------------------------------------------------------------
// OGLW App
class debugdraw : public App {
public:
    debugdraw() : App({"OGLW::debugdraw", false, false, 800, 600}) {}
    void update(float _dt) override;
    void render(float _dt) override;
    void init() override;
};
OGLWMain(debugdraw);

void debugdraw::init() {
    m_camera.setPosition({0.0, -1.0, 10.0});
}

void debugdraw::update(float _dt) {
    oglwUpdateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f, 20.f);
}

void debugdraw::render(float _dt) {
    oglwDrawDebugAxisTriad(0.3, 2.0);
    oglwDrawDebugCube(glm::vec3(0.0), OGLW::rgb(0.5, 0.7, 0.5));
    oglwDrawDebugSphere(glm::vec3(0.0), OGLW::rgb(1.0, 0.0, 0.5), 15.0);
    oglwDrawDebugXZSquareGrid(-10.0, 10.0, 0.0, 1.0);
    oglwDrawDebugScreenText("Debug draw demo", glm::vec2(10.0, 15.0), OGLW::rgb(1.0), 0.5);
}
