#pragma once

#include <string>
#include "glTypes.h"

namespace OGLW {

struct VertexAttrib {
    std::string name;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLvoid* offset;
};

} // OGLW