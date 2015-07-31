#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "error.h"

#ifdef OGLW_DEBUG
#define GL_CHECK(stmt) do { stmt; _Error::GLError(#stmt, __FILE__, __LINE__); } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif
