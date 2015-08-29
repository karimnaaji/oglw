
#include "app.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "text/glfontstash.h"
#include "gl/renderState.h"
#include "core/types.h"
#include "core/log.h"
#include "gl/gl.h"

namespace OGLW {

App::App(std::string _name, std::string _font, int _width, int _height) :
    m_name(_name), m_font(_font), m_width(_width), m_height(_height), m_textRendering(true) {
    initGLFW();
}

App::App(std::string _name, int _width, int _height) :
    m_name(_name), m_width(_width), m_height(_height), m_textRendering(false) {
    initGLFW();
}

App::~App() {
    INFO("App destroy\n");

    if (m_textRendering) {
        glfonsDelete(m_fontContext);
    }
}

void App::initGLFW() {
    INFO("App init\n");

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);

    if (!m_window) {
        ERROR("Window creation failure\n");
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
        ERROR("glewInit failed\n");
    }

    RenderState::initialize();

    if (m_textRendering) {
        GLFONSparams params;
        params.useGLBackend = true;
        m_fontContext = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT, params, nullptr);
        INFO("Loading font %s\n", m_font.c_str());
        int font = fonsAddFont(m_fontContext, m_font.c_str(), m_font.c_str());

        if (font == FONS_INVALID) {
            ERROR("Error loading font file %s\n", m_font.c_str());
            return;
        }

        glfonsScreenSize(m_fontContext, m_width * m_dpiRatio, m_height * m_dpiRatio);
        fonsSetBlur(m_fontContext, 2.5);
        fonsSetBlurType(m_fontContext, FONS_EFFECT_DISTANCE_FIELD);
    }
}


fsuint App::displayText(float _size, glm::vec2 _position, const std::string& _text, bool _clear) {
    if (!m_textRendering) {
        return 0;
    }

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
    if (!m_textRendering) {
        return;
    }

    glfonsBufferDelete(m_fontContext, _buffer);

    for (uint i = 0; i < m_texts.size(); ++i) {
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

        if (m_textRendering && m_texts.size() > 0) {
            std::vector<fsuint> toClear;

            for (uint i = 0; i < m_texts.size(); ++i) {
                if (m_texts[i].m_clear) {
                    toClear.push_back(m_texts[i].m_buffer);
                }

                glfonsBindBuffer(m_fontContext, m_texts[i].m_buffer);
                glfonsUpdateBuffer(m_fontContext);
            }

            // render state for text drawing
            RenderState::blending(true);
            RenderState::cullFace(GL_BACK);
            RenderState::blendingFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // draw registered text
            glfonsDraw(m_fontContext);

            // cleanup now unused text ids
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

