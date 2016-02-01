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
#include "guiRenderer.h"

// gamma
#ifdef OGLW_GAMMA
#define GAMMA_H_INC_ALL
#include "Gamma/Gamma.h"
#endif

#include "variant.hpp"

// std lib
#include <string>

namespace OGLW {
    typedef glm::vec3 rgb;
    typedef glm::vec4 rgba;
}

void oglwDrawDebugCube(glm::vec3 _pos, OGLW::rgb _color = glm::vec3(1.0), glm::vec3 _dimension = glm::vec3(1.0));

// Add a point in 3D space to the debug draw queue.
// Point is expressed in world-space coordinates.
void oglwDrawDebugPoint(glm::vec3 _pos, OGLW::rgb _color, float _size = 1.0);

// Add a 3D line to the debug draw queue.
void oglwDrawDebugLine(glm::vec3 _from, glm::vec3 _to, OGLW::rgb _color = OGLW::rgb(1.0));

// Add a 2D text string as an overlay to the current view, using a built-in font.
void oglwDrawDebugScreenText(std::string _text, glm::vec2 _pos, OGLW::rgb _color, float _scaling);

// Add a 3D text label centered at the given world position that
// gets projected to screen-space. The label always faces the viewer.
// '_viewportSize' is the viewport coordinates/size, in pixels.
void oglwDrawDebugProjectedText(std::string _text,
    glm::vec3 _pos,
    OGLW::rgb _color,
    const OGLW::Camera& _camera,
    glm::vec4 _viewportSize,
    float _scaling);

// Add a set of three coordinate axis depicting the position and orientation of the given transform.
// '_size' defines the size of the arrow heads. '_length' defines the length of the arrow's base line.
void oglwDrawDebugAxisTriad(float _size, float _length, glm::mat4 _transform = glm::mat4(1.0));

// Add a 3D line with an arrow-like end to the debug draw queue.
// '_size' defines the arrow head size. '_from' and '_to' the length of the arrow's base line.
void oglwDrawDebugArrow(glm::vec3 _from, glm::vec3 _to, OGLW::rgb _color = glm::vec3(1.0), float _size = 1.0);

// Add an axis-aligned cross (3 lines converging at a point) to the debug draw queue.
// '_length' defines the length of the crossing lines.
// '_center' is the world-space point where the lines meet.
void oglwDrawDebugCross(glm::vec3 _center, float _length = 1.0);

// Add a wireframe circle to the debug draw queue.
void oglwDrawDebugCircle(glm::vec3 _center,
    glm::vec3 _normal,
    OGLW::rgb _color = glm::vec3(1.0),
    float _radius = 1.0,
    int _steps = 10.0);

// Add a wireframe plane in 3D space to the debug draw queue.
void oglwDrawDebugPlane(glm::vec3 _center,
    glm::vec3 _normal,
    OGLW::rgb _color = glm::vec3(1.0),
    float _scale = 1.0);

// Add a wireframe sphere to the debug draw queue.
void oglwDrawDebugSphere(glm::vec3 _center, OGLW::rgb _color = OGLW::rgb(1.0), float _radius = 1.0);

// Add a wireframe cone to the debug draw queue.
// The cone '_apex' is the point where all lines meet.
// The length of the '_dir' vector determines the thickness.
// '_baseRadius' & '_apexRadius' are in degrees.
void olgwDrawDebugCone(glm::vec3 _apex,
    glm::vec3 _dir,
    float _baseRadius,
    float _apexRadius,
    OGLW::rgb _color = glm::vec3(1.0));

// Add a wireframe Axis Aligned Bounding Box (AABB) to the debug draw queue.
void oglwDrawDebugAABB(glm::vec3 _mins, glm::vec3 _maxs, OGLW::rgb _color = OGLW::rgb(1.0));

// Add a wireframe camera frustum pyramid to the debug draw queue.
void oglwDrawDebugCameraFrustum(const OGLW::Camera& _camera, OGLW::rgb _color = OGLW::rgb(1.0));

// Add a vertex normal for debug visualization.
// The normal vector 'normal' is assumed to be already normalized.
void oglwDrawDebugVertexNormal(glm::vec3 _origin, glm::vec3 _normal, float _length);

// Add a "tangent basis" at a given point in world space.
// Color scheme used is: normal=WHITE, tangent=YELLOW, bi-tangent=MAGENTA.
// The normal vector, tangent and bi-tangent vectors are assumed to be already normalized.
void oglwDrawDebugTangentBasis(glm::vec3 _origin,
    glm::vec3 _normal,
    glm::vec3 _tangent,
    glm::vec3 _bitangent,
    float _length);

// Makes a 3D square grid of lines along the X and Z planes.
// '_y' defines the height in the Y axis where the grid is placed.
// The grid will go from 'mins' to 'maxs' units in both the X and Z.
// '_step' defines the gap between each line of the grid.
void oglwDrawDebugXZSquareGrid(float _mins,
    float _maxs,
    float _y,
    float _step,
    OGLW::rgb _color = glm::vec3(1.0));

// Process debug command queue
void oglwDrawDebugFlush(const OGLW::Camera& _camera);

