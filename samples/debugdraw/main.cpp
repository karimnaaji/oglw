#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;
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
    DebugRender m_debugRenderer;
};
OGLWMain(debugdraw);

void debugdraw::init() {
    m_camera.setPosition({0.0, 0.0, 10.0});
    m_debugRenderer.init();
}

void debugdraw::update(float _dt) {
    oglwUpdateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f, 20.f);
    m_debugRenderer.setMVP(m_camera.getProjectionMatrix() * m_camera.getViewMatrix());
}

void debugdraw::render(float _dt) {
    ddVec3 pos;
    pos[0] = pos[1] = pos[2] = 0;
    dd::box(pos, pos, 1.5f, 1.5f, 1.5f);
    dd::flush(_dt);
}
