#pragma once

#include "glTypes.h"
#include <unordered_map>

namespace OGLW {
namespace _Error {
    
// check for gl error and print the corresponding error code
void glError(const char* stmt, const char* fname, int line);
// get the string from the error code
std::string errorCodeString(GLenum _error);

} // _Error
} // OGLW