#pragma once

#include "gl.h"
#include "mesh.h"
#include <vector>

namespace OGLW {

    std::unique_ptr<Mesh<glm::vec2>> quad(float _size) {
        auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
            {"position", 2, GL_FLOAT, false, 0},
        }));

        auto mesh = std::unique_ptr<Mesh<glm::vec2>>(new Mesh<glm::vec2>(layout, GL_TRIANGLES));

        std::vector<glm::vec2> vertices = {
            {-_size,  _size},
            {-_size, -_size},
            { _size, -_size},
            { _size,  _size},
        };

        std::vector<int> indices = {
            0, 1, 2,
            0, 2, 3,
        };

        mesh->addVertices(std::move(vertices), std::move(indices));

        return std::move(mesh);
    }
}
