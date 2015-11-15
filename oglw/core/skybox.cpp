#include "skybox.h"
#include "geometries.h"
#include "gl/renderState.h"

namespace OGLW {

Skybox::Skybox(std::string _file) : m_file(_file) {
    static const std::string shaderProgramBundle = R"END(
        #pragma begin:vertex
        #version 330
        uniform mat4 mvp;
        uniform vec3 camPosition;

        in vec3 position;

        out vec3 f_uv;

        void main() {
            f_uv = position;
            vec4 pos = mvp * vec4(position + camPosition, 1.0);
            // force depth to 1.0
            gl_Position = pos.xyww;
        }

        #pragma end:vertex

        #pragma begin:fragment
        #version 330
        uniform samplerCube tex;

        in vec3 f_uv;
        out vec4 outColor;

        void main() {
            outColor = texture(tex, f_uv);
        }
        #pragma end:fragment
    )END";

    m_shader = std::make_unique<Shader>();
    m_shader->loadBundleSource(shaderProgramBundle);

    m_texture = std::make_unique<TextureCube>(m_file);
    m_mesh = cube();
}

void Skybox::draw(const glm::mat4& _mvp, const glm::vec3& _camPosition) {
    m_texture->bind(0);

    m_shader->setUniform("mvp", _mvp);
    m_shader->setUniform("camPosition", _camPosition);
    m_shader->setUniform("tex", 0);

    RenderState::blending(GL_FALSE);
    RenderState::depthTest(GL_TRUE);

    m_mesh->draw(*m_shader);
}

}
