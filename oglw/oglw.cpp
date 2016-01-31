#include "oglw.h"

#include <memory>

#define LAZY_INIT \
    if (!m_initialized) { oglw__init(); }

using namespace OGLW;


bool m_initialized = false;

std::unique_ptr<DebugRenderer> m_debugRenderer;

void oglw__init() {
    m_debugRenderer = std::make_unique<DebugRenderer>();
    m_debugRenderer->init();

    m_initialized = true;
}

void oglwDrawDebugCube(glm::vec3 _pos, OGLW::rgb _color, glm::vec3 _dimension) {
    LAZY_INIT

    // TODO: save state/pop state

    dd::box(&_pos[0], &_color[0], _dimension.x, _dimension.y, _dimension.z);
}

void oglwDrawDebugCube(glm::vec3 _pos, uint _color, glm::vec3 _dimension) {

}

void oglwDrawDebugFlush(const Camera& _camera) {
    m_debugRenderer->setMVP(_camera.getProjectionMatrix() * _camera.getViewMatrix());

    RenderState::push();
    dd::flush(0.f);
    RenderState::pop();
}

#undef LAZY_INIT

