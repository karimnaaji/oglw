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
    /// Initialization code goes here
}

void gui::update(float _dt) {
    /// Update code goes here
}

void gui::render(float _dt) {
    /// Render code goes here
}
