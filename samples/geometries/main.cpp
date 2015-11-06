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
        uptr<Shader> m_waterShader;
        uptr<Mesh<glm::vec4>> m_geometry;
        uptr<Mesh<glm::vec4>> m_waterGeometry;
        uptr<OGLW::Texture> m_texture;
};
OGLWMain(TestApp);

void TestApp::init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // default camera
    m_camera.setPosition({0.0, -3.0, 5.0});
    m_camera.setFar(200.f);
    m_camera.setNear(0.1f);
    m_camera.setFov(30);

    m_shader = uptr<Shader>(new Shader("default.glsl"));
    m_waterShader = uptr<Shader>(new Shader("water.glsl"));

    OGLW::TextureOptions options = {
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE,
        {GL_LINEAR, GL_LINEAR},
        {GL_REPEAT, GL_REPEAT}
    };

    m_texture = uptr<OGLW::Texture>(new Texture("perlin.png", options));

    m_geometry = plane(20.f, 20.f, 150, 150);
    m_waterGeometry = plane(20.f, 20.f, 250, 250);
}

void TestApp::update(float _dt) {
    oglwUpdateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f, 20.f);
}

void TestApp::render(float _dt) {
    glm::mat4 model = glm::rotate(glm::mat4(), (float) M_PI_2, glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix() * model;

    glm::vec3 camPos = m_camera.getPosition();
    //std::cout << camPos.x << " " << camPos.y << " " << camPos.z << std::endl;

    m_texture->bind(0);

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("tex", 0);
    m_shader->setUniform("modelView", m_camera.getViewMatrix() * model);
    m_shader->setUniform("normalMatrix", glm::inverse(glm::transpose(glm::mat3(mvp))));

    RenderState::depthTest(GL_TRUE);
    RenderState::culling(GL_TRUE);
    RenderState::cullFace(GL_BACK);
    RenderState::blending(GL_FALSE);

    m_shader->setUniform("color", glm::vec3(0.53,0.36, 0.33));
    m_shader->setUniform("extrude", 3.0f);
    m_shader->setUniform("scale", 2.0f);
    m_shader->setUniform("yOffset", 0.0f);
    m_geometry->draw(*m_shader);
    m_shader->setUniform("color", glm::vec3(0.33, 0.56, 0.43));
    m_shader->setUniform("extrude", 1.0f);
    m_shader->setUniform("scale", 1.0f);
    m_shader->setUniform("yOffset", 0.5f);
    m_geometry->draw(*m_shader);
    m_shader->setUniform("extrude", 20.0f);
    m_shader->setUniform("yOffset", 12.0f);
    m_shader->setUniform("color", glm::vec3(1.0, 1.0, 1.0));
    m_shader->setUniform("scale", 0.5f);
    m_geometry->draw(*m_shader);
    mvp = glm::translate(mvp, glm::vec3(0.0, 0.0, 0.8));

    m_waterShader->setUniform("mvp", mvp);
    m_waterShader->setUniform("time", m_globalTime);
    m_waterShader->setUniform("modelView", m_camera.getViewMatrix() * model);
    m_waterShader->setUniform("normalMatrix", glm::inverse(glm::transpose(glm::mat3(mvp))));

    RenderState::blending(GL_TRUE);
    RenderState::blendingFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //m_waterGeometry->draw(*m_waterShader);
}

