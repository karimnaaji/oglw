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
        TestApp() : App("OGLW::TestApp", /*"Roboto-Regular.ttf", */1024, 720) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<Shader> m_shader;
        uptr<Shader> m_waterShader;
        uptr<Mesh<glm::vec4>> m_geometry;
        uptr<Mesh<glm::vec4>> m_waterGeometry;
        uptr<Texture> m_texture;
        uptr<RenderTarget> m_renderTarget;
        uptr<Camera> m_reflectionCamera;
        uptr<QuadRenderer> m_quadRenderer;
};
OGLWMain(TestApp);

void TestApp::init() {

    // default camera
    m_camera.setPosition({0.0, -3.0, 5.0});
    m_camera.setFar(200.f);
    m_camera.setNear(0.1f);
    m_camera.setFov(30);

    m_reflectionCamera = uptr<Camera>(new Camera());
    m_reflectionCamera->setFar(200.0f);
    m_reflectionCamera->setNear(0.1f);
    m_reflectionCamera->setFov(30);

    m_shader = uptr<Shader>(new Shader("default.glsl"));
    m_waterShader = uptr<Shader>(new Shader("water.glsl"));

    OGLW::TextureOptions options = {
        GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE,
        {GL_LINEAR, GL_LINEAR},
        {GL_REPEAT, GL_REPEAT}
    };

    m_texture = uptr<OGLW::Texture>(new Texture("perlin.png", options));

    m_geometry = plane(20.f, 20.f, 350, 350);
    m_waterGeometry = plane(20.f, 20.f, 250, 250);

    RenderTargetSetup setup;
    setup.useDepth = true;
    m_renderTarget = std::make_unique<OGLW::RenderTarget>(setup);
    m_renderTarget->create(1024, 720);

    m_quadRenderer = uptr<QuadRenderer>(new QuadRenderer());
    m_quadRenderer->init();
}

void TestApp::update(float _dt) {

    oglwUpdateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f, 20.f);

    glm::vec2 camRotation = m_camera.rotation();
    float theta = camRotation.x * (180.0 / M_PI);
    float phi = camRotation.y * (180.0 / M_PI);
    //oglwDisplayText(24.f, {20.f, 40.f}, "X:" + std::to_string(theta) + " Y: " + std::to_string(phi), true);
}

void TestApp::render(float _dt) {
    float yWaterPlane = 1.5f;
    glm::mat4 model;
    glm::mat4 mvp;

    m_texture->bind(0);

    glEnable(GL_CLIP_PLANE0);

    glm::vec3 camPosition = m_camera.getPosition();
    camPosition.y *= -1.0f;
    camPosition.y -= 2.0 * yWaterPlane;
    m_reflectionCamera->setPosition(camPosition);
    m_reflectionCamera->setRotationX(-m_camera.getRotation().x);
    m_reflectionCamera->setRotationY(m_camera.getRotation().y);

    model = glm::rotate(glm::mat4(), (float) M_PI_2, glm::vec3(1.0, 0.0, 0.0));
    mvp = m_reflectionCamera->getProjectionMatrix() * m_reflectionCamera->getViewMatrix() * model;

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("tex", 0);
    m_shader->setUniform("clipPlane", glm::vec4(0.0, 0.0, 1.0, -yWaterPlane));
    m_shader->setUniform("modelView", m_reflectionCamera->getViewMatrix() * model);
    m_shader->setUniform("normalMatrix", glm::inverse(glm::transpose(glm::mat3(mvp))));

    RenderState::depthTest(GL_TRUE);
    RenderState::culling(GL_TRUE);
    RenderState::cullFace(GL_BACK);
    RenderState::blending(GL_FALSE);

    m_renderTarget->apply(1024, 720, 0xffffffff);

    m_geometry->draw(*m_shader);

    glDisable(GL_CLIP_PLANE0);

    RenderTarget::applyDefault(1024, 720, 0xffffffff);

    /// Draw terrain

    mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix() * model;

    m_texture->bind(0);

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("modelView", m_camera.getViewMatrix() * model);
    m_shader->setUniform("normalMatrix", glm::inverse(glm::transpose(glm::mat3(mvp))));

    RenderState::depthTest(GL_TRUE);
    RenderState::culling(GL_TRUE);
    RenderState::cullFace(GL_BACK);
    RenderState::blending(GL_FALSE);

    m_geometry->draw(*m_shader);

    /// Draw water

    m_renderTarget->getRenderTexture()->bind(0);

    m_waterShader->setUniform("mvp", mvp);
    m_waterShader->setUniform("time", m_globalTime);
    m_waterShader->setUniform("modelView", m_camera.getViewMatrix() * model);
    m_waterShader->setUniform("yWaterPlane", yWaterPlane);
    m_waterShader->setUniform("normalMatrix", glm::inverse(glm::transpose(glm::mat3(mvp))));
    m_waterShader->setUniform("screenResolution", getResolution());
    m_waterShader->setUniform("reflectionTexture", 0);

    RenderState::blending(GL_TRUE);
    RenderState::blendingFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_waterGeometry->draw(*m_waterShader);


    /// Debug draw camera framebuffer
    m_quadRenderer->render(*m_renderTarget->getRenderTexture(), getResolution(), glm::vec2(0.0), 0.35);
}

