#include "oglw.h"

template <class T>
using uptr = std::unique_ptr<T>;
using namespace OGLW;

// ------------------------------------------------------------------------------
// OGLW App
class gui : public App {
    public:
        gui() : App({"OGLW::gui", false, false, 800, 600}) {}
        void update(float _dt) override;
        void render(float _dt) override;
        void init() override;
    private:
        GuiRenderer m_renderer;
};
OGLWMain(gui);

void gui::init() {
    m_renderer.init(m_window, true);
}

void gui::update(float _dt) {
    /// Update code goes here
}

void gui::render(float _dt) {
    // TODO: wrap in oglwGuiBegin/End()
    
    m_renderer.render();

    ImVec4 clear_color = ImColor(114, 144, 154);

    static float f = 0.0f;
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Render();
}
