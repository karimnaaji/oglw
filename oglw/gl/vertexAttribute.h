#pragma once

#include <string>
#include "glTypes.h"

struct VertexAttrib {
    std::string name;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLvoid* offset;
};