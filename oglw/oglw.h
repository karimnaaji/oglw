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
#include "renderTarget.h"
#include "quadRenderer.h"

// other
#include "app.h"
#include "utils.h"

// renderer
#include "debugRenderer.h"
#include "spriteRenderer.h"
#include "quadRenderer.h"
#include "skyboxRenderer.h"

// gamma
#ifdef OGLW_GAMMA
#define GAMMA_H_INC_ALL
#include "Gamma/Gamma.h"
#endif

#include "variant.hpp"

namespace OGLW {
    typedef glm::vec3 rgb;
    typedef glm::vec4 rgba;
}

void oglwDrawDebugCube(glm::vec3 _pos, OGLW::rgb _color, glm::vec3 _dimension);

void oglwDrawDebugCube(glm::vec3 _pos, uint _color, glm::vec3 _dimension);

void oglwDrawDebugFlush(const OGLW::Camera& _camera);

