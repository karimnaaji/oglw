#include "app.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "glfontstash.h"

namespace OGLW {

App::App(std::string _name, std::string _font, int _width, int _height) :
    m_name(_name), m_font(_font), m_width(_width), m_height(_height) {
    initGLFW();
}

App::~App() {
    glfonsDelete(m_fontContext);
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

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        std::cerr << "glewInit failed." << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    GLFONSparams params;
    params.useGLBackend = true;
    m_fontContext = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT, params, nullptr);

    int font = fonsAddFont(m_fontContext, m_font.c_str(), m_font.c_str());

    if (font == FONS_INVALID) {
        std::cerr << "Error loading font file " << m_font.c_str() << std::endl;
        return;
    }

    glfonsScreenSize(m_fontContext, m_width * m_dpiRatio, m_height * m_dpiRatio);
    fonsSetBlur(m_fontContext, 2.5);
    fonsSetBlurType(m_fontContext, FONS_EFFECT_DISTANCE_FIELD);
}


fsuint App::displayText(float _size, glm::vec2 _position, const std::string& _text, bool _clear) {
    fsuint textBuffer;
    fsuint textId;

    glfonsBufferCreate(m_fontContext, &textBuffer);
    glfonsGenText(m_fontContext, 1, &textId);
    glfonsSetColor(m_fontContext, 0xffffff);

    fonsSetSize(m_fontContext, _size * m_dpiRatio);
    glfonsRasterize(m_fontContext, textId, _text.c_str());
    glfonsTransform(m_fontContext, textId, _position.x * m_dpiRatio, _position.y * m_dpiRatio, 0.0, 1.0);

    m_texts.push_back({textId, textBuffer, _clear});
    return textBuffer;
}

void App::clearText(fsuint _buffer) {
    glfonsBufferDelete(m_fontContext, _buffer);

    for (int i = 0; i < m_texts.size(); ++i) {
        if (m_texts[i].m_buffer == _buffer) {
            m_texts.erase(m_texts.begin()+i);
            break;
        }
    }
}

void App::run() {
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(m_window)) {
        double time = glfwGetTime();
        double dt = time - lastTime;

        glfwGetCursorPos(m_window, &m_cursorX, &m_cursorY);
        glfwSetCursorPos(m_window, 0, 0);

        update(dt);

        glViewport(0, 0, m_width * m_dpiRatio, m_height * m_dpiRatio);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render(dt);

        if (m_texts.size() > 0) {
            std::vector<fsuint> toClear;

            for (int i = 0; i < m_texts.size(); ++i) {
                if (m_texts[i].m_clear) {
                    toClear.push_back(m_texts[i].m_buffer);
                }

                glfonsBindBuffer(m_fontContext, m_texts[i].m_buffer);
                glfonsUpdateBuffer(m_fontContext);
            }
            glfonsDraw(m_fontContext);

            for (auto buffer : toClear) {
                clearText(buffer);
            }
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        lastTime = time;
    }
}

}

