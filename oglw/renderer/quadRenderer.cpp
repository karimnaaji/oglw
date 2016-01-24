#include "quadRenderer.h"
#include "gl/renderState.h"
#include "gl/renderTarget.h"

namespace OGLW {

QuadRenderer::QuadRenderer() {}
QuadRenderer::~QuadRenderer() {}

void QuadRenderer::init() {
    static const std::string shaderProgramBundle = R"END(
        #pragma begin:vertex
        #version 330

        in vec2 position;

        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
        }
        #pragma end:vertex

        #pragma begin:fragment
        #version 330

        uniform sampler2D tex;
        uniform vec2 resolution;
        uniform vec2 screenPosition;

        out vec4 outColour;

        void main(void) {
            vec2 uv = (gl_FragCoord.xy - screenPosition) / resolution;
            outColour = texture(tex, uv);
        }

        #pragma end:fragment
    )END";

    m_shader = std::make_unique<Shader>();
    m_shader->loadBundleSource(shaderProgramBundle);
    m_quad = quad(1.0f);
}

void QuadRenderer::render(Texture& _texture, const glm::vec2& _resolution,
            const glm::vec2& _position, const float _quadSize) {

    RenderTarget::applyDefault(_resolution.x, _resolution.y);

    glm::vec2 quadSize(_quadSize * (4.0 / 3.0));

    GL_CHECK(glViewport(_position.x, _position.y, quadSize.x, quadSize.y));

    _texture.bind(0);

    RenderState::depthTest(GL_FALSE);
    RenderState::culling(GL_TRUE);
    RenderState::cullFace(GL_BACK);
    RenderState::blending(GL_FALSE);

    m_shader->setUniform("resolution", quadSize);
    m_shader->setUniform("screenPosition", _position);
    m_shader->setUniform("tex", 0);

    m_quad->draw(*m_shader);

}
} // OGLW

