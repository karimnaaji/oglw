#pragma once

#include <string>
#include "core/camera.h"
#include "text/glfontstash.h"

// forward
struct GLFWwindow;

namespace OGLW {

// forward
class Camera;

#define OGLWMainGamma(OGLWApp, audioCB) \
int main() { \
    OGLWApp app; \
    app.init(); \
    gam::AudioIO io(256, 44100, audioCB, NULL, 2); \
    gam::Sync::master().spu(io.framesPerSecond()); \
    io.start(); \
    app.run(); \
    return 0; \
} \

#define OGLWMain(OGLWApp) \
int main() { \
    OGLWApp app; \
    app.init(); \
    app.run(); \
    return 0; \
} \

class App {
    public:
        App(std::string _name, int _width, int _height);
        App(std::string _name, std::string _font, int _width, int _height);
        virtual ~App();

        virtual void update(float _dt) = 0;
        virtual void render(float _dt) = 0;
        virtual void init() = 0;

        void run();
        glm::vec2 getResolution() { return glm::vec2(m_width, m_height); }

    protected:
        std::string m_name;
        std::string m_font;

        Camera m_camera;
        GLFWwindow* m_window;
        FONScontext* m_fontContext;

        int m_width;
        int m_height;
        int m_dpiRatio;

        bool m_textRendering;

        double m_cursorX;
        double m_cursorY;
        float m_globalTime = 0.f;

        fsuint oglwDisplayText(float _size, glm::vec2 _position, const std::string& _text, bool _clear = false);
        void oglwUpdateFreeFlyCamera(float _dt, char _back, char _forward, char _left, char _right,
            float _sensitivity = 1e-2f, float _speed = 3.f);

        struct Text {
            fsuint m_textId;
            fsuint m_buffer;
            bool m_clear;
        };

    private:
        void clearText(fsuint _id);
        void initGLFW();

        std::vector<Text> m_texts;
};

} // OGLW
