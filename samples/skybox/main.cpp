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
        uptr<Mesh<glm::vec4>> m_geometry;
        uptr<Skybox> m_skybox;
};
OGLWMain(TestApp);

void TestApp::init() {
    m_camera.setPosition({0.0, -0.5, 14.0});
    m_shader = uptr<Shader>(new Shader("default.glsl"));
    m_geometry = plane(2.5f, 2.5f, 5, 5);
    m_skybox = uptr<Skybox>(new Skybox("fulllg.png"));
}

void TestApp::update(float _dt) {
    oglwUpdateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-4f);
}

void TestApp::render(float _dt) {
    glm::mat4 model;
    glm::mat4 mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix() * model;

    m_shader->setUniform("mvp", mvp);
    m_geometry->draw(*m_shader);
    m_skybox->draw(mvp, m_camera.getPosition());
}

