#pragma once

#include "gl/glTypes.h"
#include "gl/mesh.h"
#include <vector>

namespace OGLW {

std::unique_ptr<RawMesh> cube(float _size = 1.f);

std::unique_ptr<Mesh<glm::vec4>> quad(float _size);

std::unique_ptr<Mesh<glm::vec4>> plane(float _width, float _height, uint _nw, uint _nh);

std::unique_ptr<RawMesh> axis();

std::unique_ptr<RawMesh> icosahedron(float _size = 0.5f);

struct LineVertex {
    glm::vec3 position;
    uint color;
};

auto spiral(float _resolution = 1000.f, float _freq = 15.f);

} // OGLW
