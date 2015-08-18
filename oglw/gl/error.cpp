#include "error.h"
#include "gl.h"
#include "log.h"

namespace OGLW {
namespace _Error {

void glError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    
    if(err != GL_NO_ERROR) {
        ERROR("OpenGL error %s, at %s:%i - for %s\n", errorCodeString(err).c_str(), fname, line, stmt);
    }
}

std::string errorCodeString(GLenum _error) {
    switch (_error) {
        case GL_STACK_OVERFLOW:                 return "Stack overflow";
        case GL_OUT_OF_MEMORY:                  return "Out of memory";
        case GL_INVALID_OPERATION:              return "Invalid operation";
        case GL_INVALID_VALUE:                  return "Invalid value";
        case GL_INVALID_ENUM:                   return "Invalid enum";
        case GL_NO_ERROR:                       return "No error";
        case GL_STACK_UNDERFLOW:                return "Stack underflow";
        case GL_INVALID_FRAMEBUFFER_OPERATION:  return "Invalid frame buffer operation";
        case GL_CONTEXT_LOST:                   return "Context loss";
        default:                                return "Invalid error code";
    }
}

} // _Error
} // OGLW