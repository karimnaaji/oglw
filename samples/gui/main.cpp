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
};
OGLWMain(gui);

void gui::init() {
    // Toggle gui mode by default
    App::guiMode = true;

    glClearColor(0.5, 0.5, 0.7, 1.0);
}

void gui::update(float _dt) {
    /// Update code goes here
}

void gui::render(float _dt) {
    oglwImGuiBegin();

    ImVec4 clear_color = ImColor(114, 144, 154);
    {
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    {
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    oglwImGuiFlush();
}
