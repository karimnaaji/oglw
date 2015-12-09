#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "core/utils.h"
#include "core/log.h"
#include "renderState.h"

namespace OGLW {

Texture::Texture(uint _width, uint _height, TextureOptions _options, bool _generateMipmaps) :
m_options(_options),
m_generateMipmaps(_generateMipmaps)
{
    m_glHandle = 0;
    m_dirty = false;
    m_shouldResize = false;
    m_target = GL_TEXTURE_2D;

    resize(_width, _height);
}

Texture::Texture(const std::string& _file, TextureOptions _options, bool _generateMipmaps) :
Texture(0, 0, _options, _generateMipmaps)
{
    uint size;
    uchar* data = bytesFromPath(_file.c_str(), &size);
    uchar* pixels;
    int width, height, comp;

    pixels = stbi_load_from_memory(data, size, &width, &height, &comp, STBI_rgb_alpha);

    if (!pixels || size == 0) {
        WARN("Failed to load texture image resource %s\n", _file.c_str());
        free(data);
        return;
    }

    resize(width, height);
    setData(reinterpret_cast<GLuint*>(pixels), width * height);
    update(0);

    free(data);
    stbi_image_free(pixels);
}

Texture::~Texture() {
    if (m_glHandle) {
        GL_CHECK(glDeleteTextures(1, &m_glHandle));

        if (RenderState::texture.compare(m_target, m_glHandle)) {
            RenderState::texture.init(m_target, 0, false);
        }
    }
}

void Texture::bind(GLuint _textureSlot) {
    RenderState::textureUnit(_textureSlot);
    RenderState::texture(m_target, m_glHandle);
}

void Texture::setData(const GLuint* _data, uint _dataSize) {

    if (m_data.size() > 0) {
        m_data.clear();
    }

    m_data.insert(m_data.begin(), _data, _data + _dataSize);

    m_dirty = true;
}

void Texture::generate(GLuint _textureUnit) {
    GL_CHECK(glGenTextures(1, &m_glHandle));

    bind(_textureUnit);

    GL_CHECK(glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_options.filtering.min));
    GL_CHECK(glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_options.filtering.mag));

    GL_CHECK(glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_options.wrapping.wraps));
    GL_CHECK(glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_options.wrapping.wrapt));

    if (m_options.isDepthTexture) {
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                    m_options.depthOptions.textureMode));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                    m_options.depthOptions.compareMode));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,
                    m_options.depthOptions.compareFunc));
    }
}

void Texture::update(GLuint _textureUnit) {

    if (!m_dirty) {
        return;
    }

    if (m_glHandle == 0) { // textures hasn't been initialized yet, generate it

        generate(_textureUnit);

        // if no data make sure texture is 0-filled at creation (useful for transform lookup)
        if (m_data.size() == 0) {
            m_data.resize(m_width * m_height);
            std::memset(m_data.data(), 0, m_data.size());
        }
    } else {

        bind(_textureUnit);
    }

    GLuint* data = m_data.size() > 0 ? m_data.data() : nullptr;

    // resize or push data
    if (data || m_shouldResize) {
        GL_CHECK(glTexImage2D(m_target, 0, m_options.internalFormat, m_width, m_height, 0,
                    m_options.format, m_options.type, data));
        m_shouldResize = false;

        if (data && m_generateMipmaps) {
            // generate the mipmaps for this texture
            glGenerateMipmap(m_target);
        }
    }

    // clear data
    if (data) {
        m_data.clear();
    }

    m_dirty = false;
}

void Texture::resize(const uint _width, const uint _height) {
    if (m_width == _width && m_height == _height) {
        return;
    }

    m_width = _width;
    m_height = _height;

    m_shouldResize = true;
    m_dirty = true;
}

} // OGLW
