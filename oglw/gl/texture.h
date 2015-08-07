#pragma once

#include "gl.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <cstring>

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
    Texture(uint _width, uint _height,
            TextureOptions _options = {
                GL_RGBA8, GL_RGBA, {GL_LINEAR, GL_LINEAR}, {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}}, bool _generateMipmaps = false);

    Texture(const std::string& _file,
            TextureOptions _options = {
                GL_RGBA8, GL_RGBA, {GL_LINEAR, GL_LINEAR}, {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}}, bool _generateMipmaps = false);

    virtual ~Texture();

    /* Binds the texture to the specified slot */
    void bind(GLuint _textureSlot);

    /* Perform texture updates, should be called at least once and after adding data or resizing */
    virtual void update(GLuint _textureSlot);

    /* Resize the texture */
    void resize(const uint _width, const uint _height);

    /* Width and Height texture getters */
    uint getWidth() const {
        return m_width;
    }

    uint getHeight() const {
        return m_height;
    }

    GLuint getGlHandle() {
        return m_glHandle;
    }

    void setData(const GLuint* _data, uint _dataSize);

    typedef std::pair<GLuint, GLuint> TextureSlot;

protected:
    void generate(GLuint _textureUnit);

    TextureOptions m_options;
    std::vector<GLuint> m_data;
    GLuint m_glHandle;

    bool m_dirty;
    bool m_shouldResize;
    bool m_generateMipmaps;

    uint m_width;
    uint m_height;

    GLenum m_target;

    static GLuint getTextureUnit(GLuint _slot);
};

} // OGLW
