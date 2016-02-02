#include "guiRenderer.h"

#include "gl/renderState.h"

namespace OGLW {

GuiRenderer::GuiRenderer() {}

void GuiRenderer::init(GLFWwindow* window, bool _installGlfwCallbacks) {
    m_window = window;

    static const std::string shaderBundle = R"END(
        #pragma begin:vertex
        #version 330
        uniform mat4 mvp;
        in vec2 position;
        in vec2 uv;
        in vec4 color;
        out vec2 fUV;
        out vec4 fColor;

        void main() {
            fColor = color;
            fUV = uv;
            gl_Position = mvp * vec4(position, 0.0, 1.0);
        }
        #pragma end:vertex
        #pragma begin:fragment
        #version 330
        in vec2 fUV;
        in vec4 fColor;
        out vec4 outColor;
        uniform sampler2D tex;

        void main() {
            outColor = texture(tex, fUV) * fColor;
        }
        #pragma end:fragment
    )END";

    m_shader = std::make_unique<Shader>();
    m_shader->loadBundleSource(shaderBundle);

    m_vertexLayout = std::unique_ptr<VertexLayout>(new VertexLayout({
        {"position", 2, GL_FLOAT, false, 0, AttributeLocation::position},
        {"color", 4, GL_FLOAT, false, 0, AttributeLocation::color},
        {"uv", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
    }));

    m_vao = std::make_unique<Vao>();

    // TODO: glDeleteBuffers
    GL_CHECK(glGenBuffers(1, &m_vertexBuffer));
    GL_CHECK(glGenBuffers(1, &m_indexBuffer));
    m_vao->init(m_vertexBuffer, 0, *m_vertexLayout, m_vertexLayout->getLocations());
    m_shader->bindVertexLayout(*m_vertexLayout);

    ImGuiIO& io = ImGui::GetIO();

    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    io.UserData = (void*)this;
    io.RenderDrawListsFn = &GuiRenderer::render;
    io.SetClipboardTextFn = &GuiRenderer::setClipboardText;
    io.GetClipboardTextFn = &GuiRenderer::getClipboardText;

    if (_installGlfwCallbacks) {
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, scrollCallback);
        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetCharCallback(m_window, charCallback);
    }

    // Create font texture
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    TextureOptions options;
    options.internalFormat = GL_RGBA;

    m_texture = std::make_unique<Texture>(width, height, options);
    m_texture->setData(reinterpret_cast<const GLuint*>(pixels), width * height);
    m_texture->update(0);

    io.Fonts->TexID = (void *)(intptr_t)m_texture->getGlHandle();
}

const char* GuiRenderer::getClipboardText() {
    GuiRenderer* self = (GuiRenderer*)ImGui::GetIO().UserData;
    return glfwGetClipboardString(self->m_window);
}

void GuiRenderer::setClipboardText(const char *text) {
    GuiRenderer* self = (GuiRenderer*)ImGui::GetIO().UserData;
    glfwSetClipboardString(self->m_window, text);
}

void GuiRenderer::mouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/) {
    GuiRenderer* self = (GuiRenderer*)ImGui::GetIO().UserData;

    if (action == GLFW_PRESS && button >= 0 && button < 3) {
        self->m_mousePressed[button] = true;
    }
}

void GuiRenderer::scrollCallback(GLFWwindow*, double /*xoffset*/, double yoffset) {
    GuiRenderer* self = (GuiRenderer*)ImGui::GetIO().UserData;
    self->m_mouseWheel += (float)yoffset; // Use fractional mouse wheel, 1.0 unit 5 lines.
}

void GuiRenderer::keyCallback(GLFWwindow*, int key, int, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();

    if (action == GLFW_PRESS) {
        io.KeysDown[key] = true;
    }

    if (action == GLFW_RELEASE) {
        io.KeysDown[key] = false;
    }

    (void)mods; // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
}

void GuiRenderer::charCallback(GLFWwindow*, unsigned int c) {
    ImGuiIO& io = ImGui::GetIO();

    if (c > 0 && c < 0x10000) {
        io.AddInputCharacter((unsigned short)c);
    }
}

void GuiRenderer::render() {
    ImGuiIO& io = ImGui::GetIO();

    int w, h;
    int display_w, display_h;

    glfwGetWindowSize(m_window, &w, &h);
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step
    double current_time =  glfwGetTime();
    io.DeltaTime = m_time > 0.0 ? (float)(current_time - m_time) : (float)(1.0f/60.0f);
    m_time = current_time;

    if (glfwGetWindowAttrib(m_window, GLFW_FOCUSED)) {
        double mouse_x, mouse_y;
        glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
        io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
    } else {
        io.MousePos = ImVec2(-1,-1);
    }

    for (int i = 0; i < 3; i++) {
        io.MouseDown[i] = m_mousePressed[i] || glfwGetMouseButton(m_window, i) != 0;
        m_mousePressed[i] = false;
    }

    io.MouseWheel = m_mouseWheel;
    m_mouseWheel = 0.0f;

    // Hide OS mouse cursor if ImGui is drawing it
    glfwSetInputMode(m_window, GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    
    // Start the frame
    ImGui::NewFrame();
}

void GuiRenderer::render(ImDrawData* _drawData) {
    GuiRenderer* self = (GuiRenderer*)ImGui::GetIO().UserData;
    const float width = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;

    const glm::mat4 orthographicMatrix = {
        { 2.0f/width,   0.0f,       0.0f,   0.0f },
        {  0.0f,    2.0f/-height,   0.0f,   0.0f },
        {  0.0f,        0.0f,      -1.0f,   0.0f },
        { -1.0f,        1.0f,       0.0f,   1.0f },
    };

    self->m_texture->bind(0);
    self->m_shader->setUniform("mvp", orthographicMatrix);
    self->m_shader->setUniform("tex", 0);

    self->m_vao->bind();

    RenderState::depthTest(GL_FALSE);
    RenderState::culling(GL_FALSE);

    for (int n = 0; n < _drawData->CmdListsCount; n++) {
        const ImDrawList* cmdList = _drawData->CmdLists[n];
        const ImDrawIdx* idxBufferOffset = 0;

        GLsizeiptr vertexSize = (GLsizeiptr)cmdList->VtxBuffer.size() * sizeof(ImDrawVert);
        GLsizeiptr indicesSize = (GLsizeiptr)cmdList->IdxBuffer.size() * sizeof(ImDrawIdx);

        // TODO: use glMapBuffer instead
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, self->m_vertexBuffer));

        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexSize, NULL, GL_DYNAMIC_DRAW));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexSize,
            (GLvoid*)&cmdList->VtxBuffer.front(), GL_DYNAMIC_DRAW));

        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_indexBuffer));

        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, NULL, GL_DYNAMIC_DRAW));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize,
            (GLvoid*)&cmdList->IdxBuffer.front(), GL_DYNAMIC_DRAW));

        for (const ImDrawCmd* pcmd = cmdList->CmdBuffer.begin(); pcmd != cmdList->CmdBuffer.end(); pcmd++) {
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmdList, pcmd);
            } else {
                // TODO: check utility of this?
                //RenderState::texture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);

                // TODO: add to render states
                GL_CHECK(glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w),
                            (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)));

                GL_CHECK(glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                    sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idxBufferOffset));
            }

            idxBufferOffset += pcmd->ElemCount;
        }
    }
}

} // OGLW
