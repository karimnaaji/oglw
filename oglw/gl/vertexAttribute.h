#pragma once

#include <string>
#include "gl/glTypes.h"

namespace OGLW {

struct VertexAttrib {
    std::string name;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLvoid* offset;
    GLuint location = -1;
};

} // OGLW