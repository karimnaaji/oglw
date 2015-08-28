#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "gl/glTypes.h"
#include "gl/vertexAttribute.h"

namespace OGLW {

class Shader;

class VertexLayout {
public:
    VertexLayout(std::vector<VertexAttrib> _attribs);
    ~VertexLayout();

    // enable the vertex layout for a set of locations keyed by name
    void enable(const std::unordered_map<std::string, GLuint>& _locations, size_t byteOffset);
    // disable the vertex layout for a set of locations keyed by name
    void disable(const std::unordered_map<std::string, GLuint>& _locations);
    // get the stride of the vertex layout in bytes
    GLint getStride() const { return m_stride; };
    // get the vertex attributes of the vertex layout
    const std::vector<VertexAttrib>& getAttributes() { return m_attribs; };

private:
    std::vector<VertexAttrib> m_attribs;
    GLint m_stride;
};

} // OGLW
