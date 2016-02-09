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
        {"uv", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
        {"color", 4, GL_UNSIGNED_BYTE, true, 0, AttributeLocation::color},
    }));

    // TODO: glDeleteBuffers
    GL_CHECK(glGenBuffers(1, &m_vertexBuffer));
    GL_CHECK(glGenBuffers(1, &m_indexBuffer));

    m_vao = std::make_unique<Vao>();

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

    loadTheme();

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

void GuiRenderer::loadTheme() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowMinSize             = ImVec2( 160, 20 );
	style.FramePadding              = ImVec2( 4, 2 );
	style.ItemSpacing               = ImVec2( 6, 2 );
	style.ItemInnerSpacing          = ImVec2( 6, 4 );
	style.Alpha			            = 0.8f;
	style.WindowFillAlphaDefault    = 1.0f;
	style.WindowRounding            = 0.0f;
	style.FrameRounding             = 0.0f;
	style.IndentSpacing             = 6.0f;
	style.ItemInnerSpacing		    = ImVec2( 2, 4 );
	style.ColumnsMinSpacing         = 50.0f;
	style.GrabMinSize		        = 14.0f;
	style.GrabRounding		        = 0.0f;
	style.ScrollbarSize		        = 12.0f;
	style.ScrollbarRounding	        = 0.0f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.61f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.47f, 0.77f, 0.83f, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Column]                = ImVec4(0.47f, 0.77f, 0.83f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.86f, 0.93f, 0.89f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.86f, 0.93f, 0.89f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	style.Colors[ImGuiCol_TooltipBg]             = ImVec4(0.47f, 0.77f, 0.83f, 0.72f);
	style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
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

void GuiRenderer::newFrame() {
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
    RenderState::blending(GL_TRUE);
    RenderState::blendingFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _drawData->ScaleClipRects(ImGui::GetIO().DisplayFramebufferScale);

    // TODO: add to render states
    GL_CHECK(glBlendEquation(GL_FUNC_ADD));
    GL_CHECK(glEnable(GL_SCISSOR_TEST));

    for (int n = 0; n < _drawData->CmdListsCount; n++) {
        const ImDrawList* cmdList = _drawData->CmdLists[n];
        const ImDrawIdx* idxBufferOffset = 0;

        GLsizeiptr vertexSize = (GLsizeiptr)cmdList->VtxBuffer.size() * sizeof(ImDrawVert);
        GLsizeiptr indicesSize = (GLsizeiptr)cmdList->IdxBuffer.size() * sizeof(ImDrawIdx);

        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, self->m_vertexBuffer));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexSize,
            (GLvoid*)&cmdList->VtxBuffer.front(), GL_STREAM_DRAW));

        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_indexBuffer));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize,
            (GLvoid*)&cmdList->IdxBuffer.front(), GL_STREAM_DRAW));

        for (const ImDrawCmd* pcmd = cmdList->CmdBuffer.begin(); pcmd != cmdList->CmdBuffer.end(); pcmd++) {
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmdList, pcmd);
            } else {

                // TODO: add to render states
                GL_CHECK(glScissor((int)pcmd->ClipRect.x,
                                   (int)(height - pcmd->ClipRect.w),
                                   (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                                   (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)));

                GL_CHECK(glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                    sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idxBufferOffset));
            }

            idxBufferOffset += pcmd->ElemCount;
        }
    }

    GL_CHECK(glDisable(GL_SCISSOR_TEST));

    self->m_vao->unbind();
}

} // OGLW
