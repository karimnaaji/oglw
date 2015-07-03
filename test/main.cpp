#include <vector>
#include <memory>
#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;

class TestApp : public OGLW::App {
    public:
        TestApp() : OGLW::App("OGLW::TestApp", 960, 720) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<OGLW::Shader> m_shader;
        uptr<OGLW::Shader> m_backgroundShader;
        uptr<OGLW::Texture> m_texture;
        std::vector<uptr<OGLW::RawMesh>> m_meshes;
        uptr<OGLW::Mesh<glm::vec2>> m_quad;

        float m_xrot = 0.f, m_yrot = 0.f;
};

int main() {
    TestApp app;

    app.init();
    app.run();
    return 0;
}

void TestApp::init() {
    glClearColor(0.18f, 0.18f, 0.22f, 1.0f);

    m_camera.setPosition({0.0, -0.5, 14.0});

    m_shader = uptr<OGLW::Shader>(new OGLW::Shader("default.frag", "default.vert"));
    m_meshes = OGLW::loadOBJ("suzanne.obj");
    m_quad = OGLW::quad(1.f);
    m_backgroundShader = uptr<OGLW::Shader>(new OGLW::Shader("background.frag", "background.vert"));
    m_texture = uptr<OGLW::Texture>(new OGLW::Texture("lightprobe.jpg"));

    displayText(30.f, {150.f, 150.f}, "test App");
}

void TestApp::update(float _dt) {
    m_xrot += m_cursorX;
    m_yrot += m_cursorY;
}

void TestApp::render(float _dt) {
    glm::mat4 model;
    glm::mat4 view = m_camera.getViewMatrix();

    model = glm::rotate(model, m_xrot * 1e-2f, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, m_yrot * 1e-2f, glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 mvp = m_camera.getProjectionMatrix() * view * model;
    glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(view)));

    glDepthMask(GL_FALSE);
    m_backgroundShader->setUniform("resolution", {m_width, m_height});
    m_quad->draw(*m_backgroundShader);
    glDepthMask(GL_TRUE);

    m_texture->bind(0);

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("mv", model * view);
    m_shader->setUniform("normalmat", normalMat);
    m_shader->setUniform("tex", 0);

    for (auto& m : m_meshes) {
        m->draw(*m_shader);
    }
}

