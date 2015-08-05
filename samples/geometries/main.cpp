#include <vector>
#include <string>
#include <memory>
#include "oglw.h"

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
        std::unique_ptr<Shader> m_shader;
        std::unique_ptr<RawMesh> m_axis;
        std::unique_ptr<Mesh<OGLW::LineVertex>> m_geometry;
        float m_xrot = 0.f, m_yrot = 0.f;
};
OGLWMain(TestApp);

void TestApp::init() {
    m_camera.setPosition({0.0, -0.5, 7.0});

    m_shader = std::unique_ptr<Shader>(new Shader("default.frag", "default.vert"));
    m_axis = axis();
    m_geometry = spiral();
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

    m_shader->setUniform("mvp", mvp);

    m_axis->draw(*m_shader);
    m_geometry->draw(*m_shader);
}

