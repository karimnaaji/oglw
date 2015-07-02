#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace OGLW {

    Texture::Texture(unsigned int _width, unsigned int _height, TextureOptions _options) : m_options(_options) {

        m_glHandle = 0;
        m_dirty = false;
        m_shouldResize = false;
        m_target = GL_TEXTURE_2D;

        resize(_width, _height);
    }

    Texture::Texture(const std::string& _file, TextureOptions _options) : Texture(0, 0, _options) {

        unsigned int size;
        unsigned char* data = bytesFromPath(_file.c_str(), &size);
        unsigned char* pixels;
        int width, height, comp;

        pixels = stbi_load_from_memory(data, size, &width, &height, &comp, STBI_rgb_alpha);

        resize(width, height);
        setData(reinterpret_cast<GLuint*>(pixels), width * height);
        update(0);

        free(data);
        stbi_image_free(pixels);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_glHandle);
    }

    void Texture::bind(GLuint _textureSlot) {
        glActiveTexture(getTextureUnit(_textureSlot));
        glBindTexture(m_target, m_glHandle);
    }

    void Texture::setData(const GLuint* _data, unsigned int _dataSize) {

        if (m_data.size() > 0) {
            m_data.clear();
        }

        m_data.insert(m_data.begin(), _data, _data + _dataSize);

        m_dirty = true;
    }

    void Texture::generate(GLuint _textureUnit) {
        glGenTextures(1, &m_glHandle);

        bind(_textureUnit);

        glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_options.m_filtering.m_min);
        glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_options.m_filtering.m_mag);

        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_options.m_wrapping.m_wraps);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_options.m_wrapping.m_wrapt);
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
            glTexImage2D(m_target, 0, m_options.m_internalFormat, m_width, m_height, 0, m_options.m_format,
                         GL_UNSIGNED_BYTE, data);
            m_shouldResize = false;
        }

        // clear cpu data
        if (data) {
            m_data.clear();
        }

        m_dirty = false;
    }

    void Texture::resize(const unsigned int _width, const unsigned int _height) {
        m_width = _width;
        m_height = _height;

        m_shouldResize = true;
        m_dirty = true;
    }

    GLuint Texture::getTextureUnit(GLuint _unit) {
        if (_unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
            std::cout << "Warning: trying to access unavailable texture unit" << std::endl;
        }

        return GL_TEXTURE0 + _unit;
    }
}
