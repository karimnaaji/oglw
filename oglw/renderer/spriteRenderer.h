#pragma once

#include "gl/shader.h"
#include "gl/texture.h"
#include "glm/glm.hpp"
#include "core/geometries.h"

namespace OGLW {

struct Sprite {
};

struct SpriteBatch {
    Sprite loadSprite(std::string _filename) {}
};

class SpriteRenderer {
public:
    SpriteRenderer();
    ~SpriteRenderer();

    void init();
    void resize(const glm::vec2& _resolution);
    void render(const SpriteBatch& _batch);
    std::unique_ptr<SpriteBatch> createSpriteBatch(std::string _batchName);

private:
    glm::vec2 m_quadPosition;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh<glm::vec4>> m_quad;

};

} // OGLW