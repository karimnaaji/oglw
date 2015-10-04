#pragma once

#include "gl/gl.h"
#include "core/types.h"
#include <vector>
#include <memory>
#include <string>
#include <cstring>

namespace OGLW {

struct TextureFiltering {
    TextureFiltering() {}
    TextureFiltering(GLenum _min, GLenum _mag) : min(_min), mag(_mag) {}
    GLenum min = GL_LINEAR;
    GLenum mag = GL_LINEAR;
};

struct TextureWrapping {
    TextureWrapping() {}
    TextureWrapping(GLenum _wraps, GLenum _wrapt) : wraps(_wraps), wrapt(_wrapt) {}
    GLenum wraps = GL_CLAMP_TO_EDGE;
    GLenum wrapt = GL_CLAMP_TO_EDGE;
};

struct DepthTextureOptions {
    DepthTextureOptions() {}
    DepthTextureOptions(GLenum _textureMode, GLenum _compareMode, GLenum _compareFunc) :
    textureMode(_textureMode), compareMode(_compareMode), compareFunc(_compareFunc) {} 
    GLenum textureMode = GL_INTENSITY;
    GLenum compareMode = GL_TEXTURE_COMPARE_MODE;
    GLenum compareFunc = GL_LEQUAL;
};

struct TextureOptions {
    TextureOptions() {}
    TextureOptions(GLenum _internalFormat, GLenum _format, GLenum _type, TextureFiltering _filtering, 
        TextureWrapping _wrapping, bool _isDepthTexture = false, DepthTextureOptions _depthOptions = {}) :
    internalFormat(_internalFormat), format(_format), type(_type), filtering(_filtering), wrapping(_wrapping),
    isDepthTexture(_isDepthTexture), depthOptions(_depthOptions) {}
    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    TextureFiltering filtering;
    TextureWrapping wrapping;
    bool isDepthTexture = false;
    DepthTextureOptions depthOptions;
};

class Texture {

public:
    Texture(uint _width, uint _height, TextureOptions _options = {}, bool _generateMipmaps = false);
    Texture(const std::string& _file, TextureOptions _options = {}, bool _generateMipmaps = false);

    virtual ~Texture();

    // binds the texture to the specified slot
    void bind(GLuint _textureSlot);
    // perform texture updates, should be called at least once and after adding data or resizing
    virtual void update(GLuint _textureSlot);
    // resize the texture
    void resize(const uint _width, const uint _height);
    // get the width of the texture
    uint getWidth() const { return m_width; }
    // get the height of the texture
    uint getHeight() const { return m_height; }
    // returns the gl handle
    GLuint getGlHandle() { return m_glHandle; }
    // set data for the texture, needs to be updated after this call
    void setData(const GLuint* _data, uint _dataSize);

protected:
    // generate the gl handle and bind it at the specified unit
    void generate(GLuint _textureUnit);

    TextureOptions m_options;
    std::vector<GLuint> m_data;
    GLuint m_glHandle;
    GLenum m_target;
    bool m_dirty;
    bool m_shouldResize;
    bool m_generateMipmaps;
    uint m_width;
    uint m_height;
};

} // OGLW

