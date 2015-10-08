#pragma once

#include <string>
#include "gl/glTypes.h"

namespace OGLW {

enum AttributeLocation : char {
    position = 0,
    uv,
    color,
    normal
};

struct VertexAttrib {
    std::string name;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLvoid* offset;
    GLuint location = -1;
};

} // OGLW