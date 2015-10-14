#include <vector>
#include <string>
#include <memory>
#include "oglw.h"
#include "simplex.h"

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
        TestApp() : App("OGLW::TestApp", 800, 600) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;

    private:
        uptr<Shader> m_shader;
        uptr<Mesh<VoxelVert>> m_geometry;
};
OGLWMain(TestApp);

void TestApp::init() {
    m_camera.setPosition({0.0, 0.0, 10.0});
    m_shader = uptr<Shader>(new Shader("default.glsl"));

    std::vector<VoxelVert> vertices;

    //for (int x = 0; x < 10; x++) {
    //    for (int y = 0; y < 50; ++y) {
    //        for (int z = 0; z < 100; ++z) {
    //            float n = simplex_noise(1, x, y, z);
    //            vertices.push_back({{x, y, z}, {n, n, n}});
    //        }
    //    }
    //}

    uint size;
    uchar* data = bytesFromPath("terrain.png", &size);
    uchar* pixels;
    int width, height, comp;

    pixels = stbi_load_from_memory(data, size, &width, &height, &comp, STBI_rgb_alpha);

    uint nbPixels = width * height * comp;

    float heightScale = 4.0;

    for (int i = 0; i < width; i += comp) {
        for (int j = 0; j < height * 2; j += comp) {
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
    updateFreeFlyCamera(_dt, 'S', 'W', 'A', 'D', 1e-3f);
}

void TestApp::render(float _dt) {
    glm::mat4 mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();

    RenderState::culling(GL_FALSE);
    RenderState::depthTest(GL_TRUE);
    RenderState::depthWrite(GL_TRUE);

    m_shader->setUniform("mvp", mvp);

    m_geometry->draw(*m_shader);
}

