#pragma once

#include <string>
#include <map>
#include <memory>
#include "imgui.h"
#include "imgui_internal.h"
#include "gl/shader.h"
#include "gl/vao.h"
#include "gl/texture.h"
#include "gl/vertexLayout.h"

namespace OGLW {

class GuiRenderer {
public:
        GuiRenderer();
        ~GuiRenderer();

        void init(GLFWwindow* window, bool _installGlfwCallbacks);
        void newFrame();

        static void setClipboardText(const char* _text);
        static const char* getClipboardText();
        static void mouseButtonCallback(GLFWwindow*, int _button, int _action, int /*mods*/);
        static void scrollCallback(GLFWwindow*, double /*xoffset*/, double _yoffset);
        static void keyCallback(GLFWwindow*, int _key, int, int _action, int _mods);
        static void charCallback(GLFWwindow*, unsigned int _c);

        ImFont* getFont(const std::string& _name);

private:
        static void render(ImDrawData* _drawData);

        void loadTheme();

        std::unique_ptr<Shader> m_shader;
        std::unique_ptr<VertexLayout> m_vertexLayout;
        std::unique_ptr<Vao> m_vao;
        std::unique_ptr<Texture> m_texture;
        GLuint m_vertexBuffer;
        GLuint m_indexBuffer;

        bool m_mousePressed[3] = { false, false, false };
        float m_mouseWheel = 0.0f;
        double m_time = 0.0f;

        std::map<std::string, ImFont*> m_Fonts;
        GLFWwindow* m_window;
};

} // OGLW
