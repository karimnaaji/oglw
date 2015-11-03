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
        TestApp() : App("OGLW::TestApp", 1024, 720) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<Shader> m_shader;
        uptr<Mesh<glm::vec4>> m_geometry;
        uptr<OGLW::Texture> m_texture;
};
OGLWMain(TestApp);

void TestApp::init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    m_camera.setPosition({0.0, 0.0, 5.0});
    m_camera.setFov(45.f);

    m_shader = uptr<Shader>(new Shader("default.glsl"));

    OGLW::TextureOptions options = {
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE,
        {GL_LINEAR, GL_LINEAR},
        {GL_REPEAT, GL_REPEAT}
    };

    m_texture = uptr<OGLW::Texture>(new OGLW::Texture("perlin.png", options));

    m_geometry = plane(40.f, 40.f, 350, 350);
}

void TestApp::update(float _dt) {
    updateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f, 20.f);
}

void TestApp::render(float _dt) {
    glm::mat4 mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();

    m_texture->bind(0);

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("tex", 0);
    m_shader->setUniform("time", m_globalTime);

    RenderState::depthTest(GL_TRUE);
    RenderState::culling(GL_FALSE);

    m_geometry->draw(*m_shader);
}

