#pragma once

// defines
#define OGLW_DEBUG
#define GLFONS_DEBUG

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "error.h"
#include "log.h"

namespace OGLW {

#ifdef OGLW_DEBUG
#define GL_CHECK(STMT) do { STMT; _Error::GLError(#STMT, __FILE__, __LINE__); } while (0)
#else
#define GL_CHECK(STMT) STMT;
#endif

}
