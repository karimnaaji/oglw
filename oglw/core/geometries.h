#pragma once

#include "gl/glTypes.h"
#include "gl/mesh.h"
#include <vector>

namespace OGLW {

std::unique_ptr<RawMesh> cube(float _size = 1.f);

std::unique_ptr<Mesh<glm::vec4>> quad(float _size);

std::unique_ptr<Mesh<glm::vec4>> plane(float _width, float _height, uint _nw, uint _nh);


} // OGLW
