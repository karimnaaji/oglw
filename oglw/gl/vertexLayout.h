#pragma once

#include <vector>
#include <string>
#include "gl/glTypes.h"
#include "gl/vertexAttribute.h"

namespace OGLW {

class Shader;

class VertexLayout {
public:
    VertexLayout(std::vector<VertexAttrib> _attribs);
    ~VertexLayout();

    void enable(const Shader& _program, size_t byteOffset);
    void disable(const Shader& _program);
    GLint getStride() const { return m_stride; };

private:
    std::vector<VertexAttrib> m_attribs;
    GLint m_stride;
};

} // OGLW
