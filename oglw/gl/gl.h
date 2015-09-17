#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "gl/error.h"
#include "gl/trace.h"

namespace OGLW {

#ifdef OGLW_DEBUG
#ifdef OGLW_TRACE
#define TRACE(STMT, FILE, LINE) _Trace::glTrace(STMT, FILE, LINE);
#else
#define TRACE(STMT, FILE, LINE)
#endif
#define GL_CHECK(STMT) do { \
    STMT; \
    TRACE(#STMT, __FILE__, __LINE__); \
    _Error::glError(#STMT, __FILE__, __LINE__); \
} while (0)
#else
#define GL_CHECK(STMT) STMT;
#endif

static void printGLTrace() { 
#ifdef OGLW_TRACE
    _Trace::printGLTrace();
#endif
}

} // OGLW
