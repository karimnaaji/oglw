#include <vector>
#include <string>
#include <memory>
#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;
using namespace OGLW;

// ------------------------------------------------------------------------------
// OGLW App
class TestApp : public OGLW::App {
    public:
        TestApp() : OGLW::App("OGLW::TestApp", 800, 600) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<Mesh<glm::vec4>> m_plane;
        uptr<Mesh<glm::vec4>> m_quad;
        std::vector<uptr<RawMesh>> m_cubes;
        uptr<Shader> m_shader;
};
OGLWMain(TestApp);

void TestApp::init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    m_camera.setPosition({0.0, -0.5, 14.0});
    m_plane = plane(30.f, 30.f, 1, 1);
    for (int i = 0; i < 64; ++i) {
        m_cubes.push_back(std::move(cube(0.3)));
    }
    m_shader = uptr<Shader>(new Shader("default.glsl"));
}

void TestApp::update(float _dt) {
    updateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f);
}

void TestApp::render(float _dt) {
    glm::mat4 model, mvp, view, proj;

    model = glm::mat4();
    view = m_camera.getViewMatrix();
    proj = m_camera.getProjectionMatrix();

    int i = 0;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                const auto& cube = m_cubes[i++];
                model = glm::translate(glm::mat4(), glm::vec3(x, y, z));
                mvp = proj * view * model;
                m_shader->setUniform("mvp", mvp);
                cube->draw(*m_shader);
            }
        }
    }
}

