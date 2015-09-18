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
        uptr<Shader> m_fullQuadShader;
        uptr<Shader> m_shadowCasterShader;
        uptr<Shader> m_depthWrite;
        uptr<RawMesh> m_mesh;
        uptr<RawMesh> m_shadowCasterMesh;
        uptr<Mesh<glm::vec4>> m_plane;
        uptr<Mesh<glm::vec4>> m_quad;
        uptr<RenderTarget> m_renderTarget;
        uptr<Camera> m_shadowCasterCamera;
};
OGLWMain(TestApp);

void TestApp::init() {
    /// Setup cameras

    // shadow caster camera
    m_shadowCasterCamera = uptr<Camera>(new Camera);
    m_shadowCasterCamera->setFov(52);
    m_shadowCasterCamera->setNear(0.5f);
    m_shadowCasterCamera->setFar(5.0f);

    // default camera
    m_camera.setPosition({0.0, 0.0, 15.0});
    m_camera.setFar(200.f);
    m_camera.setNear(0.1f);
    m_camera.setFov(50);

    /// Setup shaders
    m_shader = uptr<Shader>(new Shader("default.glsl"));
    m_fullQuadShader = uptr<Shader>(new Shader("fullquad.glsl"));
    m_shadowCasterShader = uptr<Shader>(new Shader("no-shading.glsl"));
    m_depthWrite = uptr<Shader>(new Shader("depth-write.glsl"));

    /// Setup render target
    RenderTargetSetup setup;
    setup.useDepthTexture = true;
    m_renderTarget = std::make_unique<OGLW::RenderTarget>(setup);
    m_renderTarget->create(800, 600);

    /// Setup meshes
    m_mesh = loadOBJ("tile.blend");
    m_plane = plane(30.f, 30.f, 1, 1);
    m_shadowCasterMesh = cube(0.05);
    m_quad = quad(1.f);

    // temporary fix on unfind attributes
    m_mesh->draw(*m_shader);
}

void TestApp::update(float _dt) {
    updateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f);
}

void TestApp::render(float _dt) {
    glm::mat4 model, mvp, view;
    glm::mat3 normalMat;

    /// Apply first render target to draw scene depth from light caster
    glm::vec3 shadowCasterPos = glm::vec3(0.0, 0.1, 2.5);
    m_shadowCasterCamera->setPosition(shadowCasterPos);

    glm::mat4 depthMVP = m_shadowCasterCamera->getProjectionMatrix() * m_shadowCasterCamera->getViewMatrix();

    m_renderTarget->apply(2048, 2048);
    RenderState::depthWrite(GL_TRUE);
    RenderState::culling(GL_TRUE);
    RenderState::cullFace(GL_FRONT);
    m_depthWrite->setUniform("depthMVP", depthMVP);
    m_mesh->draw(*m_depthWrite);
    RenderState::cullFace(GL_BACK);
    m_depthWrite->setUniform("depthMVP", depthMVP);
    m_plane->draw(*m_depthWrite);

    /// Default render target drawing
    RenderState::depthWrite(GL_TRUE);

    RenderTarget::applyDefault(800, 600, false);

    model = glm::mat4();
    view = m_camera.getViewMatrix();
    mvp = m_camera.getProjectionMatrix() * view;
    normalMat = glm::transpose(glm::inverse(glm::mat3(view)));

    m_renderTarget->bindRenderTexture(0);
    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("depthMVP", depthMVP);
    m_shader->setUniform("depthMap", 0);
    m_shader->setUniform("normalMat", normalMat);
    m_shader->setUniform("lightPos", shadowCasterPos);

    RenderState::culling(GL_TRUE);
    RenderState::cullFace(GL_FRONT);
    m_mesh->draw(*m_shader);
    m_shader->setUniform("mvp", m_camera.getProjectionMatrix() * view * glm::translate(model, {0.0, 0.0, -0.01}));
    RenderState::culling(GL_FALSE);
    m_plane->draw(*m_shader);

    /// Draw shadow caster position
    model = glm::translate(glm::mat4(), shadowCasterPos);
    mvp = m_camera.getProjectionMatrix() * view * model;
    m_shadowCasterShader->setUniform("mvp", mvp);
    m_shadowCasterMesh->draw(*m_shadowCasterShader);
}

