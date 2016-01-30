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
private:
    DebugRenderer m_debugRenderer;
};
OGLWMain(debugdraw);

void debugdraw::init() {
    m_camera.setPosition({0.0, -1.0, 10.0});
    m_debugRenderer.init();
}

void debugdraw::update(float _dt) {
    oglwUpdateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f, 20.f);
    m_debugRenderer.setMVP(m_camera.getProjectionMatrix() * m_camera.getViewMatrix());
}

void debugdraw::render(float _dt) {
    static const ddVec3 col  = { 0.0f, 0.8f, 0.8f };
    static const ddVec3 pos = { 0.0f, 0.0f, 0.0f };
    static const ddMat4x4 identity = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // TODO: wrap dd::* into DebugRenderer

    dd::axisTriad(identity, 0.3f, 2.0f);
    dd::box(pos, col, 1.5f, 1.5f, 1.5f);
    dd::sphere(pos, col, 15.0f);
    dd::xzSquareGrid(-10.0, 10.0, 0.0, 1.0, col);

    const ddVec3 textPos2D = { 10.0f, 15.0f, 0.0f };
    dd::screenText("Debug draw demo", textPos2D, col);

    dd::flush(_dt);
}
