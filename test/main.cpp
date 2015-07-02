#include <string>
#include <vector>
#include <memory>

#include "oglw.h"

class TestApp : public OGLW::App {
    public:
        TestApp() : OGLW::App("test", 1024, 768) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        std::unique_ptr<OGLW::Shader> m_shader;
        std::unique_ptr<OGLW::Texture> m_texture;
        std::vector<std::unique_ptr<OGLW::Mesh<OGLW::Vertex>>> m_meshes;

        float m_xrot = 0.f, m_yrot = 0.f;
};

int main() {
    TestApp app;

    app.init();
    app.run();

    return 0;
}

void TestApp::update(float _dt) {
    double cursorX, cursorY;
    glfwGetCursorPos(m_window, &cursorX, &cursorY);
    glfwSetCursorPos(m_window, 0, 0);

    if (std::abs(cursorX) <= 100 && std::abs(cursorY) <= 100) {
        m_xrot += cursorX;
        m_yrot += cursorY;
    }
}

void TestApp::init() {
    glClearColor(0.18f, 0.18f, 0.22f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    m_shader = std::unique_ptr<OGLW::Shader>(new OGLW::Shader("default.frag", "default.vert"));

    m_meshes = OGLW::loadOBJ("suzanne.obj");
    m_camera.setPosition({0.0, -0.5, 14.0});
    m_texture = std::unique_ptr<OGLW::Texture>(new OGLW::Texture("lightprobe.jpg"));
}

void TestApp::render(float _dt) {
    glm::mat4 model;
    glm::mat4 view = m_camera.getViewMatrix();

    model = glm::rotate(model, m_xrot * 1e-2f, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, m_yrot * 1e-2f, glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 mvp = m_camera.getProjectionMatrix() * view * model;
    glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(view)));

    m_texture->bind(0);

    m_shader->use();
    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("mv", model * view);
    m_shader->setUniform("normalmat", normalMat);
    m_shader->setUniform("tex", 0);

    for (auto& m : m_meshes) {
        m->draw(*m_shader);
    }
}

