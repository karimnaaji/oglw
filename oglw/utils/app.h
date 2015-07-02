#pragma once

#include <string>
#include "gl.h"
#include "camera.h"

namespace OGLW {

    class App {
        public:
            App(std::string _name, int _width, int _height);
            virtual ~App() {}

            virtual void update(float _dt) = 0;
            virtual void render(float _dt) = 0;
            virtual void init() = 0;

            void run();

        protected:
            std::string m_name;
            Camera m_camera;
            GLFWwindow* m_window;
            int m_width;
            int m_height;
            int m_dpiRatio;

        private:
            void initGLFW();
    };

}
