#include <vector>
#include <string>
#include <memory>
#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;
using namespace OGLW;
using namespace gam;

// ------------------------------------------------------------------------------
// Gamma sound processing configuration
Sine<> src(220);
Pan<> pan;
LFO<> panMod(0.5);
float s;

void audioCB(AudioIOData& io) {
    while(io()) {
        s = src() * 0.2;
        pan.pos(panMod.tri());
        float2 xy = pan(s);
        io.out(0) = xy.x;
        io.out(1) = xy.y;
    }
}

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

        float m_xrot = 0.f, m_yrot = 0.f;
};
OGLWMainGamma(TestApp, audioCB);

void TestApp::init() {
    glClearColor(0.1, 0.1, 0.25, 1.0);
    m_camera.setPosition({0.0, -1.0, 18.0});

    m_shader = uptr<Shader>(new Shader("default.glsl"));

    OGLW::TextureOptions options = {
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE,
        {GL_LINEAR, GL_LINEAR},
        {GL_REPEAT, GL_REPEAT}
    };

    m_texture = uptr<OGLW::Texture>(new OGLW::Texture("perlin.png", options));

    m_geometry = plane(20.f, 20.f, 150, 150);
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

    m_texture->bind(0);

    m_shader->setUniform("mvp", mvp);    
    m_shader->setUniform("tex", 0);
    m_shader->setUniform("t0", s);
    m_shader->setUniform("time", m_globalTime);

    RenderState::depthTest(GL_TRUE);
    RenderState::culling(GL_FALSE);

    m_geometry->draw(*m_shader);
}

