#include "geometries.h"


namespace OGLW {

std::unique_ptr<RawMesh> cube(float _size) {

    auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"color", 3, GL_FLOAT, false, 0, AttributeLocation::color},
        {"normal", 3, GL_FLOAT, false, 0, AttributeLocation::normal},
        {"uv", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
    }));

    auto mesh = std::unique_ptr<RawMesh>(new RawMesh(layout, GL_TRIANGLES));

    std::vector<int> indices = {
        5, 1, 3, 3, 7, 5, // +x
        6, 2, 0, 0, 4, 6, // -x
        2, 6, 7, 7, 3, 2, // +y
        5, 4, 0, 0, 1, 5, // -y
        0, 2, 3, 3, 1, 0, // +z
        7, 6, 4, 4, 5, 7  // -z
    };

    std::vector<Vertex> vertices;
    vertices.reserve(8);

    vertices.push_back({{-1.0 * _size, -1.0 * _size,  1.0 * _size}});
    vertices.push_back({{ 1.0 * _size, -1.0 * _size,  1.0 * _size}});
    vertices.push_back({{-1.0 * _size,  1.0 * _size,  1.0 * _size}});
    vertices.push_back({{ 1.0 * _size,  1.0 * _size,  1.0 * _size}});
    vertices.push_back({{-1.0 * _size, -1.0 * _size, -1.0 * _size}});
    vertices.push_back({{ 1.0 * _size, -1.0 * _size, -1.0 * _size}});
    vertices.push_back({{-1.0 * _size,  1.0 * _size, -1.0 * _size}});
    vertices.push_back({{ 1.0 * _size,  1.0 * _size, -1.0 * _size}});

    mesh->addVertices(std::move(vertices), std::move(indices));
    return std::move(mesh);
}

std::unique_ptr<Mesh<glm::vec4>> quad(float _size) {
    auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
        {"position", 2, GL_FLOAT, false, 0, AttributeLocation::position},
        {"uv", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
    }));

    auto mesh = std::unique_ptr<Mesh<glm::vec4>>(new Mesh<glm::vec4>(layout, GL_TRIANGLES));

    std::vector<glm::vec4> vertices = {
        {-_size,  _size, 0.0, 1.0},
        {-_size, -_size, 0.0, 0.0},
        { _size, -_size, 1.0, 0.0},
        { _size,  _size, 1.0, 1.0},
    };

    std::vector<int> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    mesh->addVertices(std::move(vertices), std::move(indices));

    return std::move(mesh);
}

std::unique_ptr<Mesh<glm::vec4>> plane(float _width, float _height, uint _nw, uint _nh) {
    auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
        {"position", 2, GL_FLOAT, false, 0, AttributeLocation::position},
        {"uv", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
    }));

    auto mesh = std::unique_ptr<Mesh<glm::vec4>>(new Mesh<glm::vec4>(layout, GL_TRIANGLES));

    std::vector<glm::vec4> vertices;
    std::vector<int> indices;

    int indexOffset = 0;

    float rw = 1.f / _width;
    float rh = 1.f / _height;
    float ow = _width / _nw;
    float oh = _height / _nh;

    float ou = ow * rw;
    float ov = oh * rh;

    for (float w = -_width / 2.0; w < _width / 2.0; w += ow) {
        for (float h = -_height / 2.0; h < _height / 2.0; h += oh) {
            float u = (w + _width / 2.0) * rw;
            float v = (h + _height / 2.0) * rh;

            glm::vec4 v0 = {w, h + oh, u, v + ov};
            glm::vec4 v1 = {w, h, u, v};
            glm::vec4 v2 = {w + ow, h, u + ou, v};
            glm::vec4 v3 = {w + ow, h + oh, u + ou, v + ov};

            vertices.push_back(v0);
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 3);

            indexOffset += 4;
        }
    }

    mesh->addVertices(std::move(vertices), std::move(indices));

    return std::move(mesh);
}

}
