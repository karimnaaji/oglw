#pragma once

#include "texture.h"
#include "stb_image.h"
#include <vector>
#include <string>
#include <cstring>

namespace OGLW {

class TextureCube : public Texture {

public:
    TextureCube(std::string _file,
                TextureOptions _options = {
                    GL_RGBA, GL_RGBA, {GL_LINEAR, GL_LINEAR}, {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE}});

    // generate gl handle and upload the texture data if not done
    void update(GLuint _textureUnit) override;
    // resize the texture cube, deleted
    void resize(const uint _width, const uint _height) = delete;
    // set data to the texture cube, deleted
    void setData(const GLuint* _data, uint _dataSize) = delete;

private:
    // load the horizontal crossed-texture cube from a file
    void load(const std::string& _file);
    
    struct Face {
        GLenum m_face;
        std::vector<uint> m_data;
        int m_offset;
    };

    const GLenum CubeMapFace[6]{GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

    std::vector<Face> m_faces;
};

} // OGLW
