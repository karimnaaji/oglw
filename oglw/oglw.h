#pragma once

#include "types.h"

// math utils
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "gl.h"
#include "shader.h"
#include "tiny_obj_loader.h"
#include "mesh.h"
#include "camera.h"
#include "texture.h"
#include "geometries.h"
#include "renderState.h"
#include "skybox.h"
#include "renderTarget.h"
#include "quadRenderer.h"

// other
#include "app.h"
#include "utils.h"
#include "debugRender.h"

// gamma
#ifdef OGLW_GAMMA
#define GAMMA_H_INC_ALL
#include "Gamma/Gamma.h"
#endif

#include "variant.hpp"

