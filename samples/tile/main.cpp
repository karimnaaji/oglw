#include <vector>
#include <string>
#include <memory>
#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;
using namespace OGLW;

// ------------------------------------------------------------------------------
// OGLW App
class TestApp : public App {
    public:
        TestApp() : App("OGLW::TestApp", 800, 600) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<Shader> m_shader;
        uptr<Texture> m_texture;
        uptr<RawMesh> m_mesh;
        uptr<Mesh<glm::vec4>> m_plane;

        float m_xrot = 0.f, m_yrot = 0.f;
};
OGLWMain(TestApp);

void TestApp::init() {
    m_camera.setPosition({0.0, -0.5, 14.0});

    m_shader = uptr<Shader>(new Shader("default.glsl"));
    m_mesh = loadOBJ("tile.blend");
    m_texture = uptr<Texture>(new Texture("lightprobe.jpg"));
    m_plane = plane(30.f, 30.f, 1, 1);
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

    RenderState::culling(GL_TRUE);
    RenderState::cullFace(GL_FRONT);

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("mv", model * view);
    m_shader->setUniform("normalmat", normalMat);
    m_shader->setUniform("tex", 0);

    m_mesh->draw(*m_shader);

    RenderState::cullFace(GL_BACK);

    model = glm::translate(model, glm::vec3(0.0, 0.0, -0.01));
    mvp = m_camera.getProjectionMatrix() * view * model;
    m_shader->setUniform("mvp", mvp);
    m_plane->draw(*m_shader);
}

