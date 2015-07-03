#pragma once

#include <string>
#include "gl.h"
#include "camera.h"
#include "glfontstash.h"

namespace OGLW {

    class App {
        public:
            App(std::string _name, int _width, int _height);
            virtual ~App();

            virtual void update(float _dt) = 0;
            virtual void render(float _dt) = 0;
            virtual void init() = 0;

            void run();

        protected:
            std::string m_name;
            Camera m_camera;
            GLFWwindow* m_window;
            FONScontext* m_fontContext;
            int m_width;
            int m_height;
            int m_dpiRatio;

            fsuint displayText(float _size, glm::vec2 _position, const std::string& _text);
            void clearText(fsuint _id);

            struct Text {
                fsuint m_textId;
                fsuint m_buffer;
            };

        private:
            void initGLFW();

            std::vector<Text> m_texts;
    };

}
