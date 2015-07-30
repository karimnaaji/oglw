#pragma once

#include <vector>
#include <string>
#include <map>
#include "gl.h"
#include "shader.h"

namespace OGLW {

class Shader;

class VertexLayout {
public:
    struct VertexAttrib {
        std::string name;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLvoid* offset;
    };

    VertexLayout(std::vector<VertexAttrib> _attribs);

    virtual ~VertexLayout();

    void enable(const Shader& _program, size_t byteOffset);
    void disable(const Shader& _program);

    GLint getStride() const {
        return m_stride;
    };
    std::string getDefaultVertShader();
    std::string getDefaultFragShader();

private:
    std::vector<VertexAttrib> m_attribs;
    GLint m_stride;
};

} // OGLW
