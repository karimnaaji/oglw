#pragma once

#include "gl/glTypes.h"
#include "gl/mesh.h"
#include <vector>

namespace OGLW {

static std::unique_ptr<RawMesh> cube(float _size = 1.f) {

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

static std::unique_ptr<Mesh<glm::vec4>> quad(float _size) {
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

static std::unique_ptr<Mesh<glm::vec4>> plane(float _width, float _height, uint _nw, uint _nh) {
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

static std::unique_ptr<RawMesh> axis() {

    auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"color", 3, GL_FLOAT, false, 0, AttributeLocation::color},
        {"normal", 3, GL_FLOAT, false, 0, AttributeLocation::normal},
        {"uv", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
    }));

    auto mesh = std::unique_ptr<RawMesh>(new RawMesh(layout, GL_LINES));

    std::vector<Vertex> vertices;
    vertices.reserve(6);

    vertices.push_back({{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}});
    vertices.push_back({{1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}}); // x (blue)
    vertices.push_back({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}});
    vertices.push_back({{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}}); // y (green)
    vertices.push_back({{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}});
    vertices.push_back({{0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}}); // z (red)

    mesh->addVertices(std::move(vertices), {});
    return std::move(mesh);
}


static std::unique_ptr<RawMesh> icosahedron(float _size = 0.5f) {
    auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"color", 3, GL_FLOAT, false, 0, AttributeLocation::color},
        {"normal", 3, GL_FLOAT, false, 0, AttributeLocation::normal},
        {"uv", 2, GL_FLOAT, false, 0, AttributeLocation::uv},
    }));

    auto mesh = std::unique_ptr<RawMesh>(new RawMesh(layout, GL_LINE_STRIP));

    std::vector<Vertex> vertices;
    vertices.reserve(12);

    const float phi = (1.0f + sqrt(5.0f)) * 0.5f * _size;

    vertices.push_back({{-1.0,  phi,  0.0}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ 0.0,  phi,  0.0}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ 0.0,  1.0, -phi}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ 0.0,  1.0,  phi}, {1.0, 1.0, 1.0}});
    vertices.push_back({{-phi,  0.0, -1.0}, {1.0, 1.0, 1.0}});
    vertices.push_back({{-phi,  0.0,  1.0}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ phi,  0.0, -1.0}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ phi,  0.0,  1.0}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ 0.0, -1.0, -phi}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ 0.0, -1.0,  phi}, {1.0, 1.0, 1.0}});
    vertices.push_back({{-1.0, -phi,  0.0}, {1.0, 1.0, 1.0}});
    vertices.push_back({{ 1.0, -phi,  0.0}, {1.0, 1.0, 1.0}});

    std::vector<int> indices = {
        0, 1, 2, 0, 3, 1, 0, 4, 5,
        1, 7, 6, 1, 6, 2, 1, 3, 7,
        0, 2, 4, 0, 5, 3, 2, 6, 8,
        2, 8, 4, 3, 5, 9, 3, 9, 7,
        11, 6, 7, 10, 5, 4, 10, 4, 8,
        10, 9, 5, 11, 8, 6, 11, 7, 9,
        10, 8, 11, 10, 11, 9
    };

    mesh->addVertices(std::move(vertices), std::move(indices));
    return std::move(mesh);
}

struct LineVertex {
    glm::vec3 position;
    uint color;
};

static auto spiral(float _resolution = 1000.f, float _freq = 15.f) {
    auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
        {"position", 3, GL_FLOAT, false, 0, AttributeLocation::position},
        {"color", 4, GL_UNSIGNED_BYTE, true, 0, AttributeLocation::color},
    }));

    auto mesh = std::unique_ptr<Mesh<LineVertex>>(new Mesh<LineVertex>(layout, GL_LINE_STRIP));
    std::vector<LineVertex> vertices;

    float off = 1.0 / _resolution;
    for (float i = 0.0; i < 1.0 + off; i += off) {
        float s = cos(i * 2.0 * M_PI + M_PI) * 0.5f + 0.5f;
        vertices.push_back({{cos(i * M_PI * _freq) * s, i, sin(i * M_PI * _freq) * s}, 0xffffffff});
    }

    mesh->addVertices(std::move(vertices), {});
    return std::move(mesh);
}

} // OGLW
