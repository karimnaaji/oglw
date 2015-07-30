#pragma once

// math utils
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

// gl
#define OGLW_DEBUG
#define GLFONS_DEBUG

#include "gl.h"
#include "shader.h"
#include "mesh.h"
#include "tiny_obj_loader.h"
#include "camera.h"
#include "texture.h"
#include "geometries.h"
#include "renderState.h"

// other
#include "app.h"
#include "utils.h"

// gamma
#define GAMMA_H_INC_ALL
#include "Gamma/gamma.h"
