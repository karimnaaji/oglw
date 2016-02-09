#pragma once

#include <string>
#include "core/camera.h"

struct GLFWwindow;

namespace OGLW {

// forward
class Camera;

#ifdef OGLW_GAMMA
#define OGLWMainGamma(OGLWApp, audioCB) \
int main() { \
    OGLWApp app; \
    app.initGLFW(); \
    app.init(); \
    gam::AudioIO io(256, 44100, audioCB, NULL, 2); \
    gam::Sync::master().spu(io.framesPerSecond()); \
    io.start(); \
    app.run(); \
    return 0; \
}
#endif

#define OGLWMain(OGLWApp) \
int main() { \
    OGLWApp app; \
    app.initGLFW(); \
    app.init(); \
    app.run(); \
    return 0; \
} \

struct AppConfiguration {
    std::string name;
    bool resize;
    bool fullscreen;
    uint width;
    uint height;
};

class App {
    public:
        App(AppConfiguration _config);
        virtual ~App();

        virtual void update(float _dt) = 0;
        virtual void render(float _dt) = 0;
        virtual void init() = 0;
        virtual void onWindowResize() {}
        void run();
        void initGLFW();

        glm::vec2 resolution();
        glm::vec2 physicalResolution();
        
        static bool guiMode;

    protected:
        AppConfiguration m_config;
        Camera m_camera;
        GLFWwindow* m_window;
        double m_cursorX;
        double m_cursorY;
        int m_dpiRatio;
        float m_globalTime;

        void oglwUpdateFreeFlyCamera(float _dt, char _back, char _forward, char _left, char _right,
            float _sensitivity = 1e-2f, float _speed = 3.f);
};

} // OGLW
