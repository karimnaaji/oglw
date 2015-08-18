#pragma once

#include "glTypes.h"
#include "vboMesh.h"
#include <vector>
#include <memory>
#include "tiny_obj_loader.h"

namespace OGLW {

template <class T> class Mesh : public VboMesh {
public:
    Mesh(std::shared_ptr<VertexLayout> _vertexLayout, GLenum _drawMode) : VboMesh(_vertexLayout, _drawMode){};

    void addVertices(std::vector<T>&& _vertices, std::vector<int>&& _indices) {
        vertices.push_back(_vertices);
        indices.push_back(_indices);

        m_nVertices += _vertices.size();
        m_nIndices += _indices.size();
    }

    virtual void compileVertexBuffer() override {
        compile(vertices, indices);
    }

    void updateVertices(GLintptr _byteOffset, uint _nVerts, const T& _newVertexValue);

    template<class A>
    void updateAttribute(GLintptr _byteOffset, uint _nVerts, const A& _newAttributeValue) {
        if (!m_isCompiled) {
            return;
        }

        size_t aSize = sizeof(A);
        size_t tSize = sizeof(T);

        // updating an attribute for _nVerts vertex means updating a byte distance
        // of (_nVerts - 1) * tSize in the buffer
        size_t attrByteDist = (_nVerts - 1) * tSize;

        if ((size_t)(attrByteDist + _byteOffset) > m_nVertices * tSize) {
            return;
        }

        // update the vertices attributes
        for (size_t i = 0; i < _nVerts; ++i) {
            std::memcpy(m_glVertexData + _byteOffset + i * tSize, &_newAttributeValue, aSize);
        }

        // set dirty from _byteOffset by the distance between the two attributes + the size of it
        setDirty(_byteOffset, attrByteDist + aSize);
    }

protected:

    void setDirty(GLintptr _byteOffset, GLsizei _byteSize);

    std::vector<std::vector<T>> vertices;
    std::vector<std::vector<int>> indices;
};

template<class T>
void Mesh<T>::setDirty(GLintptr _byteOffset, GLsizei _byteSize) {
    if (!m_dirty) {
        m_dirtySize = _byteSize;
        m_dirtyOffset = _byteOffset;
        m_dirty = true;
    } else {
        GLsizei dBytes = std::abs(_byteOffset - m_dirtyOffset);
        GLintptr nOff = _byteOffset + _byteSize;
        GLintptr pOff = m_dirtySize + m_dirtyOffset;

        if (_byteOffset < m_dirtyOffset) {
            // update before the old buffer offset
            m_dirtyOffset = _byteOffset;

            // merge sizes
            if (nOff > pOff) {
                m_dirtySize = _byteSize;
            } else {
                m_dirtySize += dBytes;
            }

            m_dirty = true;

        } else if (nOff > pOff) {
            // update starting after the old buffer offset
            m_dirtySize = dBytes + _byteSize;
            m_dirty = true;
        }
    }
}

template<class T>
void Mesh<T>::updateVertices(GLintptr _byteOffset, uint _nVerts, const T& _newVertexValue) {
    if (!m_isCompiled) {
        return;
    }

    size_t tSize = sizeof(T);

    if (_nVerts * tSize + _byteOffset > m_nVertices * tSize) {
            return;
    }

    // update the vertices
    for (int i = 0; i < _nVerts; ++i) {
        std::memcpy(m_glVertexData + _byteOffset + i * tSize, &_newVertexValue, tSize);
    }

    setDirty(_byteOffset, _nVerts * tSize);
}

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uvs;
};

typedef Mesh<Vertex> RawMesh;

std::vector<std::unique_ptr<RawMesh>> loadOBJ(std::string _path) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err = tinyobj::LoadObj(shapes, materials, _path.c_str(), NULL);

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    auto layout = std::shared_ptr<OGLW::VertexLayout>(new OGLW::VertexLayout({
        {"position", 3, GL_FLOAT, false, 0},
        {"color", 3, GL_FLOAT, false, 0},
        {"normal", 3, GL_FLOAT, false, 0},
        {"uv", 2, GL_FLOAT, false, 0},
    }));

    std::vector<std::unique_ptr<RawMesh>> meshes;

    for (size_t i = 0; i < shapes.size(); i++) {
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        bool hasNormals;
        bool hasUVs;

        auto mesh = std::unique_ptr<RawMesh>(new RawMesh(layout, GL_TRIANGLES));
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            indices.push_back(shapes[i].mesh.indices[3 * f + 0]);
            indices.push_back(shapes[i].mesh.indices[3 * f + 1]);
            indices.push_back(shapes[i].mesh.indices[3 * f + 2]);
        }
        assert((shapes[i].mesh.positions.size() % 3) == 0);

        if (shapes[i].mesh.normals.size() == 0) {
            hasNormals = false;
        }

        if (shapes[i].mesh.texcoords.size() == 0) {
            hasUVs = false;
        }

        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            glm::vec2 uv;
            glm::vec3 normal;
            glm::vec3 position;
            glm::vec3 color;

            if (hasUVs) {
                uv = {
                    shapes[i].mesh.texcoords[3 * v + 0],
                    shapes[i].mesh.texcoords[3 * v + 1]
                };
            }

            if (hasNormals) {
                normal = {
                    shapes[i].mesh.normals[3 * v + 0],
                    shapes[i].mesh.normals[3 * v + 1],
                    shapes[i].mesh.normals[3 * v + 2]
                };
            }

            position = {
                shapes[i].mesh.positions[3 * v + 0],
               -shapes[i].mesh.positions[3 * v + 1],
                shapes[i].mesh.positions[3 * v + 2]
            };

            vertices.push_back({ position, color, normal, uv });
        }

        if (!hasNormals) {
            std::vector<glm::vec3> verts;
            verts.resize(vertices.size());
            for (int j = 0; j < vertices.size(); ++j) {
                verts[j] = vertices[j].position;
            }
            auto normals = VboMesh::computeNormals(verts, indices);
            for (int j = 0; j < vertices.size(); ++j) {
                vertices[j].normal = normals[j];
            }
        }

        mesh->addVertices(std::move(vertices), std::move(indices));
        meshes.push_back(std::move(mesh));
    }

    return std::move(meshes);
}

} // OGLW
