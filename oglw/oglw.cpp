#include "oglw.h"

#include "glm/gtc/type_ptr.hpp"
#include <memory>

#define LAZY_INIT \
    if (!m_initialized) { oglw__init(); }

using namespace OGLW;

GLFWwindow* glfwWindow = nullptr;

bool m_initialized = false;

std::unique_ptr<DebugRenderer> m_debugRenderer;

std::unique_ptr<GuiRenderer> m_imguiRenderer;

void oglw__init() {
    m_debugRenderer = std::make_unique<DebugRenderer>();
    m_debugRenderer->init();

    m_imguiRenderer = std::make_unique<GuiRenderer>();
    m_imguiRenderer->init(glfwWindow, false);

    m_initialized = true;
}

void oglwDrawDebugCube(glm::vec3 _pos, OGLW::rgb _color, glm::vec3 _dimension) {
    LAZY_INIT

    dd::box(&_pos[0], &_color[0], _dimension.x, _dimension.y, _dimension.z);
}

void oglwDrawDebugPoint(glm::vec3 _pos, OGLW::rgb _color, float _size) {
    LAZY_INIT

    dd::point(&_pos[0], &_color[0], _size);
}

void oglwDrawDebugLine(glm::vec3 _from, glm::vec3 _to, OGLW::rgb _color) {
    LAZY_INIT

    dd::line(&_from[0], &_to[0], &_color[0]);
}

void oglwDrawDebugScreenText(std::string _text, glm::vec2 _pos, OGLW::rgb _color, float _scaling) {
    LAZY_INIT

    dd::screenText(_text, &_pos[0], &_color[0], _scaling);
}

void oglwDrawDebugProjectedText(std::string _text,
    glm::vec3 _pos,
    OGLW::rgb _color,
    const OGLW::Camera& _camera,
    glm::vec4 _viewportSize,
    float _scaling)
{
    LAZY_INIT

    glm::mat4 vpMatrix = _camera.getProjectionMatrix() * _camera.getViewMatrix();
    dd::projectedText(_text, &_pos[0], &_color[0], glm::value_ptr(vpMatrix), _viewportSize[0], _viewportSize[1],
            _viewportSize[2], _viewportSize[3], _scaling);
}

void oglwDrawDebugAxisTriad(float _size, float _length, glm::mat4 _transform) {
    LAZY_INIT

    dd::axisTriad(glm::value_ptr(_transform), _size, _length);
}

void oglwDrawDebugArrow(glm::vec3 _from, glm::vec3 _to, OGLW::rgb _color, float _size) {
    LAZY_INIT

    dd::arrow(&_from[0], &_to[0], &_color[0], _size);
}

void oglwDrawDebugCross(glm::vec3 _center, float _length) {
    LAZY_INIT

    dd::cross(&_center[0], _length);
}

void oglwDrawDebugCircle(glm::vec3 _center, glm::vec3 _normal, OGLW::rgb _color, float _radius, int _steps) {
    LAZY_INIT

    dd::circle(&_center[0], &_normal[0], &_color[0], _radius, _steps);
}

void oglwDrawDebugPlane(glm::vec3 _center, glm::vec3 _normal, OGLW::rgb _color, float _scale) {
    LAZY_INIT

    dd::plane(&_center[0], &_normal[0], &_color[0], &_color[0], _scale, 1.0f);
}

void oglwDrawDebugSphere(glm::vec3 _center, OGLW::rgb _color, float _radius) {
    LAZY_INIT

    dd::sphere(&_center[0], &_color[0], _radius);
}


void olgwDrawDebugCone(glm::vec3 _apex, glm::vec3 _dir, float _baseRadius, float _apexRadius, OGLW::rgb _color) {
    LAZY_INIT

    dd::cone(&_apex[0], &_dir[0], &_color[0], _baseRadius, _apexRadius);
}

void oglwDrawDebugAABB(glm::vec3 _mins, glm::vec3 _maxs, OGLW::rgb _color) {
    LAZY_INIT

    dd::aabb(&_mins[0], &_maxs[0], &_color[0]);
}

void oglwDrawDebugCameraFrustum(const Camera& _camera, OGLW::rgb _color) {
    LAZY_INIT

    glm::mat4 invClipMatrix = glm::inverse(_camera.getProjectionMatrix() * _camera.getViewMatrix());
    dd::frustum(glm::value_ptr(invClipMatrix), &_color[0]);
}

void oglwDrawDebugVertexNormal(glm::vec3 _origin, glm::vec3 _normal, float _length) {
    LAZY_INIT

    dd::vertexNormal(&_origin[0], &_normal[0], _length);
}

void oglwDrawDebugTangentBasis(glm::vec3 _origin,
    glm::vec3 _normal,
    glm::vec3 _tangent,
    glm::vec3 _bitangent,
    float _length)
{
    LAZY_INIT

    dd::tangentBasis(&_origin[0], &_normal[0], &_tangent[0], &_bitangent[0], _length);
}

void oglwDrawDebugXZSquareGrid(float _mins, float _maxs, float _y, float _step, OGLW::rgb _color) {
    LAZY_INIT

    dd::xzSquareGrid(_mins, _maxs, _y, _step, &_color[0]);
}

void oglwDrawDebugFlush(const Camera& _camera) {
    LAZY_INIT

    m_debugRenderer->setMVP(_camera.getProjectionMatrix() * _camera.getViewMatrix());

    dd::flush(0.f);
}

void oglwImGuiBegin() {
    LAZY_INIT

    m_imguiRenderer->newFrame();

}

void oglwImGuiFlush() {
    LAZY_INIT

    ImGui::Render();
}

OGLW::GuiRenderer& oglwGetGuiRenderer() {
    LAZY_INIT

    return *m_imguiRenderer;
}

#undef LAZY_INIT

