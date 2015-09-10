#include <vector>
#include <string>
#include <memory>
#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;

// ------------------------------------------------------------------------------
// Gamma sound processing configuration
gam::SineDs<> src(3);
gam::Accum<> tmr(1);
gam::LFO<> modSR(1./ 8, 0.0);
gam::LFO<> modQn(1./32, 0.5);
gam::Quantizer<> qnt;
static float f;

void audioCB(gam::AudioIOData& io) {
    while (io()) {
        if (tmr()) {
            src.set(0, 220, 1, 2.0);
            src.set(1, 347, 1, 1.2);
            src.set(2, 1237, 1, 0.2);
            tmr.freq(gam::rnd::uni(2., 1.));
        }

        qnt.freq(modSR.triU() * 4000 + 1400);
        qnt.step(modQn.paraU() * 0.25);

        float s = src() * 0.2;
        s = qnt(s);
        f = s;

        io.out(0) = io.out(1) = s;
    }
}

// ------------------------------------------------------------------------------
// OGLW App
class TestApp : public OGLW::App {
    public:
        TestApp() : OGLW::App("OGLW::TestApp", 800, 600) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<OGLW::Shader> m_shader;
        uptr<OGLW::Texture> m_texture;
        uptr<OGLW::RawMesh> m_mesh;

        float m_xrot = 0.f, m_yrot = 0.f;
};
OGLWMainGamma(TestApp, audioCB);

void TestApp::init() {
    m_camera.setPosition({0.0, -0.5, 14.0});

    m_shader = uptr<OGLW::Shader>(new OGLW::Shader("default.glsl"));
    m_mesh = OGLW::loadOBJ("tile.blend");
    m_texture = uptr<OGLW::Texture>(new OGLW::Texture("lightprobe.jpg"));
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

    OGLW::RenderState::culling(GL_TRUE);
    OGLW::RenderState::cullFace(GL_FRONT);

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("mv", model * view);
    m_shader->setUniform("normalmat", normalMat);
    m_shader->setUniform("tex", 0);

    m_mesh->draw(*m_shader);
}

