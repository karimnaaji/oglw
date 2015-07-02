#include "app.h"

namespace OGLW {

    App::App(std::string _name, int _width, int _height) : m_name(_name), m_width(_width), m_height(_height) {
        initGLFW();
    }

    void App::initGLFW() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        glfwWindowHint(GLFW_SAMPLES, 4);
        m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);

        if (!m_window) {
            std::cerr << "Window creation failed" << std::endl;
            glfwTerminate();
        }

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
        glfwSetCursorPos(m_window, 0, 0);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_dpiRatio = fbWidth / m_width;

        glfwMakeContextCurrent(m_window);

        glewExperimental = GL_TRUE;
        glewInit();
    }

    void App::run() {
        double lastTime = glfwGetTime();

        while (!glfwWindowShouldClose(m_window)) {
            double time = glfwGetTime();
            double dt = time - lastTime;

            update(dt);

            glViewport(0, 0, m_width * m_dpiRatio, m_height * m_dpiRatio);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render(dt);

            glfwSwapBuffers(m_window);
            glfwPollEvents();

            lastTime = time;
        }
    }

}

