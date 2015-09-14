#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "core/utils.h"

namespace OGLW {

Camera::Camera()
    : m_position(0.0f, 0.0f, 5.0f), m_rotation(0.0f, 0.0f), m_aspectRatio(4 / 3), m_fov(50.0f), m_near(0.01f),
      m_far(1000.0f) {
}

Camera::~Camera() {
}

void Camera::setAspectRatio(float _ratio) {
    m_aspectRatio = _ratio;
}

glm::mat4 Camera::getRotationMatrix() const {
    glm::mat4 rotationMatrix;
    rotationMatrix = glm::rotate(rotationMatrix, m_rotation.x, glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, m_rotation.y, glm::vec3(0, 1, 0));
    return rotationMatrix;
}

void Camera::translate(glm::vec3 _vec) {
    m_position += _vec;
}

glm::vec3 Camera::forward() const {
    glm::vec4 forward = glm::inverse(getRotationMatrix()) * glm::vec4(0, 0, -1, 1);
    return glm::vec3(forward);
}

glm::vec3 Camera::up() const {
    glm::vec4 up = glm::inverse(getRotationMatrix()) * glm::vec4(0, 1, 0, 1);
    return glm::vec3(up);
}

glm::vec3 Camera::right() const {
    glm::vec4 right = glm::inverse(getRotationMatrix()) * glm::vec4(1, 0, 0, 1);
    return glm::vec3(right);
}

glm::vec3 Camera::getPosition() const {
    return m_position;
}

void Camera::setPosition(glm::vec3 _position) {
    m_position = _position;
}

void Camera::rotate(glm::vec2 _rotation) {
    m_rotation += _rotation;
    normalizeAngles();
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(m_fov, m_aspectRatio, m_near, m_far);
}

void Camera::setFov(float _fov) {
    m_fov = _fov;
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view = getRotationMatrix();
    view = glm::translate(view, -m_position);
    return view;
}

void Camera::lookAt(glm::vec3 _point) {
    glm::vec3 direction = glm::normalize(m_position - _point);
    m_rotation.x = rad2deg(atan2f(direction.x, direction.z));
    m_rotation.y = rad2deg(acosf(direction.y));

    normalizeAngles();
}

void Camera::normalizeAngles() {
    //m_rotation.y = fmodf(m_rotation.y, M_PI_2);
    //m_rotation.y = m_rotation.y < 0.0f ? m_rotation.y + M_PI_2 : m_rotation.y;

    if (m_rotation.x > m_maxRotationX) {
        m_rotation.x = m_maxRotationX;
    }
    if (m_rotation.x < -m_maxRotationX) {
        m_rotation.x = -m_maxRotationX;
    }
}

} // OGLW
