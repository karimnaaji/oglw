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
        uptr<OGLW::RenderTarget> m_renderTarget;
};
OGLWMain(TestApp);

void TestApp::init() {
    m_camera.setPosition({0.0, -0.5, 14.0});
    m_renderTarget = std::make_unique<RenderTarget>();
    m_renderTarget->create(800, 600);
}

void TestApp::update(float _dt) {

}

void TestApp::render(float _dt) {
    m_renderTarget->apply(800, 600);
}

