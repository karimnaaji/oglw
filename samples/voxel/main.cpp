#include <vector>
#include <string>
#include <memory>
#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;
using namespace OGLW;

struct VoxelVert {
    glm::vec3 position;
    glm::vec3 color;
};

static std::unique_ptr<Mesh<VoxelVert>> voxels(std::vector<VoxelVert>& _vertices) {
    static auto layout = std::shared_ptr<VertexLayout>(new VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"color", 3, GL_FLOAT, false, 0, AttributeLocation::color},
    }));

    auto mesh = std::make_unique<Mesh<VoxelVert>>(layout, GL_POINTS);

    mesh->addVertices(std::move(_vertices), {});
    return std::move(mesh);
}

// ------------------------------------------------------------------------------
// OGLW App
class TestApp : public App {
    public:
        TestApp() : OGLW::App({"OGLW::TestApp", false, false, 1168, 720}) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<Shader> m_shader;
        uptr<Shader> m_lightShader;
        uptr<RawMesh> m_lightMesh;
        uptr<Mesh<VoxelVert>> m_geometry;
        uptr<SkyboxRenderer> m_skybox;
};
OGLWMain(TestApp);

void TestApp::init() {
    m_camera.setPosition({0.0, 0.0, 10.0});
    m_camera.setNear(2.0);
    m_camera.setFar(500.0);
    m_camera.setFov(55);
    m_shader = uptr<Shader>(new Shader("default.glsl"));
    m_lightShader = uptr<Shader>(new Shader("no-shading.glsl"));
    m_lightMesh = cube(0.5);
    m_skybox = uptr<SkyboxRenderer>(new SkyboxRenderer("grimmnight_large.jpg"));
    m_skybox->init();

    std::vector<VoxelVert> vertices;

    uint size;
    uchar* data = bytesFromPath("terrain.png", &size);
    uchar* pixels;
    int width, height, comp;

    pixels = stbi_load_from_memory(data, size, &width, &height, &comp, STBI_rgb_alpha);

    uint nbPixels = width * height * comp;

    float heightScale = 4.0;

    for (int i = 0; i < width; i += comp) {
        for (int j = 0; j < height; j += comp) {
            uint value = pixels[j * width + i];
            int height = floor(value / heightScale);
            int j0 = (j - comp) * width + i;
            int j1 = (j + comp) * width + i;
            int i0 = j * width + i - comp;
            int i1 = j * width + i + comp;
            int ix = i / comp;
            int jy = j / comp;

            if (j0 > 0) {
                uint v0j = pixels[j0];
                if (v0j < value) {
                    for (int k = v0j; k < value; ++k) {
                        uint h = floor(k / heightScale);
                        glm::vec3 c(v0j);
                        vertices.push_back({{ix, h, jy}, c / 255.0f});
                    }
                }
            }
            if (j1 < nbPixels) {
                uint v1j = pixels[j1];
                if (v1j < value) {
                    for (int k = v1j; k < value; ++k) {
                        uint h = floor(k / heightScale);
                        glm::vec3 c(v1j);
                        vertices.push_back({{ix, h, jy}, c / 255.0f});
                    }
                }
            }
            if (i0 > 0) {
                uint v0i = pixels[i0];
                if (v0i < value) {
                    for (int k = v0i; k < value; ++k) {
                        uint h = floor(k / heightScale);
                        glm::vec3 c(v0i);
                        vertices.push_back({{ix, h, jy}, c / 255.0f});
                    }
                }
            }
            if (i1 < nbPixels) {
                uint v1i = pixels[i1];
                if (v1i < value) {
                    for (int k = v1i; k < value; ++k) {
                        uint h = floor(k / heightScale);
                        glm::vec3 c(v1i);
                        vertices.push_back({{ix, h, jy}, c / 255.0f});
                    }
                }
            }

            glm::vec3 c(value);
            vertices.push_back({{ix, height, jy}, c / 255.0f});
        }
    }

    free(data);
    stbi_image_free(pixels);

    m_geometry = voxels(vertices);
}

void TestApp::update(float _dt) {
    oglwUpdateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f, 55.f);
}

void TestApp::render(float _dt) {
    glm::vec3 lightPos = glm::vec3(cos(m_globalTime) * 5.f + 10.f, 2.f, sin(m_globalTime) * 5.f + 10.f);
    glm::mat4 mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mvp)));

    m_skybox->render(mvp, m_camera.getPosition());

    RenderState::culling(GL_FALSE);
    RenderState::depthTest(GL_TRUE);
    RenderState::depthWrite(GL_TRUE);

    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("view", m_camera.getViewMatrix());
    m_shader->setUniform("normalMatrix", normalMatrix);
    m_shader->setUniform("viewPos", m_camera.getPosition());

    m_shader->setUniform("light.position", lightPos);
    m_shader->setUniform("light.color", glm::vec3(0.6, 0.68, 0.68));
    m_shader->setUniform("light.ambiant", glm::vec3(0.7));
    m_shader->setUniform("light.diffuseIntensity", 0.5f);
    m_shader->setUniform("light.specularIntensity", 5.0f);

    m_geometry->draw(*m_shader);

    glm::mat4 model = glm::translate(glm::mat4(), lightPos);
    mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix() * model;
    m_lightShader->setUniform("mvp", mvp);
    m_lightMesh->draw(*m_lightShader);
}

