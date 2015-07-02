#pragma once

#include "gl.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>

namespace OGLW {

    struct TextureFiltering {
        GLenum m_min;
        GLenum m_mag;
    };

    struct TextureWrapping {
        GLenum m_wraps;
        GLenum m_wrapt;
    };

    struct TextureOptions {
        GLenum m_internalFormat;
        GLenum m_format;
        TextureFiltering m_filtering;
        TextureWrapping m_wrapping;
    };

    class Texture {

    public:
        Texture(unsigned int _width, unsigned int _height,
                TextureOptions _options = {
                    GL_ALPHA, GL_ALPHA, {GL_LINEAR, GL_LINEAR}, {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}});

        Texture(const std::string& _file,
                TextureOptions _options = {
                    GL_RGBA, GL_RGBA, {GL_LINEAR, GL_LINEAR}, {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}});

        virtual ~Texture();

        /* Binds the texture to the specified slot */
        void bind(GLuint _textureSlot);

        /* Perform texture updates, should be called at least once and after adding data or resizing */
        virtual void update(GLuint _textureSlot);

        /* Resize the texture */
        void resize(const unsigned int _width, const unsigned int _height);

        /* Width and Height texture getters */
        unsigned int getWidth() const {
            return m_width;
        }
        unsigned int getHeight() const {
            return m_height;
        }

        GLuint getGlHandle() {
            return m_glHandle;
        }

        void setData(const GLuint* _data, unsigned int _dataSize);

        typedef std::pair<GLuint, GLuint> TextureSlot;

    protected:
        void generate(GLuint _textureUnit);

        TextureOptions m_options;
        std::vector<GLuint> m_data;
        GLuint m_glHandle;

        bool m_dirty;
        bool m_shouldResize;

        unsigned int m_width;
        unsigned int m_height;

        GLenum m_target;

        static GLuint getTextureUnit(GLuint _slot);
    };
}