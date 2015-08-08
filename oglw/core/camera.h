#pragma once

#include "gl.h"
#include <iostream>
#include <cmath>
#include "glm/glm.hpp"

namespace OGLW {

class Camera {
public:
    Camera();
    ~Camera();

    void setAspectRatio(float _ratio);
    void setPosition(glm::vec3 _position);

    glm::vec3 forward() const;
    glm::vec3 up() const;
    glm::vec3 right() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getRotationMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;

    void translate(glm::vec3 _vec);
    void rotate(glm::vec2 _rotation);
    void setFov(float _fov);

    void lookAt(glm::vec3 _point);
    void normalizeAngles();

private:
    glm::vec3 m_position;
    glm::vec2 m_rotation;

    float m_aspectRatio;
    float m_fov;
    float m_near;
    float m_far;

    const float m_maxRotationX = 89.0f;
};

} // OGLW
