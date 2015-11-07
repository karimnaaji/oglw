#pragma once

#include "gl/shader.h"
#include "gl/texture.h"
#include "glm/glm.hpp"
#include "geometries.h"

namespace OGLW {

class QuadRenderer {
public:
    QuadRenderer();
    ~QuadRenderer();

    void init();
    void render(Texture& _texture, const glm::vec2& _resolution,
            const glm::vec2& _position = glm::vec2(0.0), const float _quadSize = 1.0f);

private:

    glm::vec2 m_quadPosition;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh<glm::vec4>> m_quad;

};

} // OGLW

