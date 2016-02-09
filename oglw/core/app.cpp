
#include "app.h"
#include "gl/renderState.h"
#include "core/types.h"
#include "core/log.h"
#include "gl/gl.h"
#include "oglw.h"

namespace OGLW {

bool App::guiMode = false;

void mouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods) {
    if (App::guiMode) {
        auto& guiRenderer = oglwGetGuiRenderer();
        guiRenderer.mouseButtonCallback(_window, _button, _action, _mods);
    }
}

void scrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset) {
    if (App::guiMode) {
        auto& guiRenderer = oglwGetGuiRenderer();
        guiRenderer.scrollCallback(_window, _xoffset, _yoffset);
    }
}

void charCallback(GLFWwindow* _window, unsigned int _c) {
    if (App::guiMode) {
        auto& guiRenderer = oglwGetGuiRenderer();
        guiRenderer.charCallback(_window, _c);
    }
}

void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) {
    if (_action == GLFW_PRESS) {
        switch (_key) {
            case GLFW_KEY_ESCAPE:
                App::guiMode = !App::guiMode;
            break;
        }
    }

    if (App::guiMode) {
        auto& guiRenderer = oglwGetGuiRenderer();
        guiRenderer.keyCallback(_window, _key, _scancode, _action, _mods);
    }
}

App::App(AppConfiguration _config) {
    m_config = _config;
    m_globalTime = 0.f;
}

App::~App() {
    INFO("App destroy\n");

    printGLTrace();
}

void App::initGLFW() {
    INFO("App init\n");

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, m_config.resize);

    glfwWindowHint(GLFW_SAMPLES, 4);

    if (m_config.fullscreen) {
        m_window = glfwCreateWindow(m_config.width, m_config.height, m_config.name.c_str(),
            glfwGetPrimaryMonitor(), NULL);
    } else {
        m_window = glfwCreateWindow(m_config.width, m_config.height, m_config.name.c_str(), NULL, NULL);
    }

    if (!m_window) {
        ERROR("Window creation failure\n");
        glfwTerminate();
    }

    glfwWindow = m_window;

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
    glfwSetCursorPos(m_window, 0, 0);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_dpiRatio = fbWidth / fbHeight;

    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;

    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    glfwSetCharCallback(m_window, charCallback);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        ERROR("glewInit failed\n");
    } else {
        // pop any error triggered by glew initialization
        GLenum error = glGetError();
        while (error != GL_NO_ERROR) {
            error = glGetError();
        }
    }

    RenderState::initialize();
}

glm::vec2 App::resolution() {
    return glm::vec2(m_config.width, m_config.height);
}

glm::vec2 App::physicalResolution() {
    return resolution() * (float)m_dpiRatio;
}

void App::run() {
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(m_window)) {
        double time = glfwGetTime();
        double dt = time - lastTime;
        m_globalTime += dt;

        if (!App::guiMode) {
            glfwGetCursorPos(m_window, &m_cursorX, &m_cursorY);
            glfwSetCursorPos(m_window, 0, 0);
        }

        glfwSetInputMode(m_window, GLFW_CURSOR, App::guiMode ?  GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);

        update(dt);

        glViewport(0, 0, m_config.width * m_dpiRatio, m_config.height * m_dpiRatio);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render(dt);
        oglwDrawDebugFlush(m_camera);

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        lastTime = time;
    }
}

void App::oglwUpdateFreeFlyCamera(float _dt, char _back, char _forward, char _left, char _right,
    float _sensitivity, float _speed) {

    if (glfwGetKey(m_window, _back)) {
        m_camera.translate(_dt * _speed * -m_camera.forward());
    } else if (glfwGetKey(m_window, _forward)) {
        m_camera.translate(_dt * _speed * m_camera.forward());
    }

    if (glfwGetKey(m_window, _right)) {
        m_camera.translate(_dt * _speed * -m_camera.right());
    } else if (glfwGetKey(m_window, _left)) {
        m_camera.translate(_dt * _speed * m_camera.right());
    }

    m_camera.rotate(glm::vec2(-_sensitivity * m_cursorY, -_sensitivity * m_cursorX));
    glfwSetCursorPos(m_window, 0, 0);
}

} // OGLW

