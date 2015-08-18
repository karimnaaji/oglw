#pragma once

#include <vector>
#include <memory>
#include <cstring>
#include "glTypes.h"
#include "vertexLayout.h"
#include "shader.h"

namespace OGLW {

#define MAX_INDEX_VALUE 65535

class VboMesh {

public:
    VboMesh(std::shared_ptr<VertexLayout> _vertexlayout, GLenum _drawMode = GL_TRIANGLES, GLenum _hint = GL_STATIC_DRAW);
    VboMesh();

    void setVertexLayout(std::shared_ptr<VertexLayout> _vertexLayout);
    void setDrawMode(GLenum _drawMode = GL_TRIANGLES);
    virtual ~VboMesh();

    int numVertices() const {
        return m_nVertices;
    }

    int numIndices() const {
        return m_nIndices;
    }

    virtual void compileVertexBuffer() = 0;

    bool upload();
    bool subDataUpload();

    void draw(const Shader& _shader);

    static std::vector<glm::vec3> computeNormals(std::vector<glm::vec3> _vertices, std::vector<int> _indices) {
        std::vector<glm::vec3> normals;
        normals.resize(_vertices.size());

        for (int i = 0; i < _indices.size() / 3; ++i) {
            int i1 = _indices[3 * i + 0];
            int i2 = _indices[3 * i + 1];
            int i3 = _indices[3 * i + 2];

            const glm::vec3& v1 = _vertices[i1];
            const glm::vec3& v2 = _vertices[i2];
            const glm::vec3& v3 = _vertices[i3];

            glm::vec3 d = glm::normalize(glm::cross(v2 - v1, v3 - v1));

            normals[i1] += d;
            normals[i2] += d;
            normals[i3] += d;
        }

        for (auto& n : normals) {
            n = glm::normalize(n);
        }

        return std::move(normals);
    }

    GLsizei getDirtySize() const { return m_dirtySize; }
    GLintptr getDirtyOffset() const { return m_dirtyOffset; }

protected:
    std::vector<std::pair<uint32_t, uint32_t>> m_vertexOffsets;
    std::shared_ptr<VertexLayout> m_vertexLayout;

    int m_nVertices;
    GLuint m_glVertexBuffer;
    GLbyte* m_glVertexData = nullptr;
    GLuint m_glVertexArray;

    int m_nIndices;
    GLuint m_glIndexBuffer;
    GLushort* m_glIndexData = nullptr;
    GLenum m_hint;
    GLenum m_drawMode;

    bool m_isUploaded;
    bool m_isCompiled;
    bool m_dirty;

    GLsizei m_dirtySize;
    GLintptr m_dirtyOffset;

    template <typename T>
    void compile(std::vector<std::vector<T>>& _vertices, std::vector<std::vector<int>>& _indices) {

        std::vector<std::vector<T>> vertices;
        std::vector<std::vector<int>> indices;

        // take over contents
        std::swap(_vertices, vertices);
        std::swap(_indices, indices);

        int vertexOffset = 0, indexOffset = 0;

        // Buffer positions: vertex byte and index short
        int vPos = 0, iPos = 0;

        int stride = m_vertexLayout->getStride();
        m_glVertexData = new GLbyte[stride * m_nVertices];

        bool useIndices = m_nIndices > 0;
        if (useIndices) {
            m_glIndexData = new GLushort[m_nIndices];
        }

        for (size_t i = 0; i < vertices.size(); i++) {
            auto curVertices = vertices[i];
            size_t nVertices = curVertices.size();
            int nBytes = nVertices * stride;

            std::memcpy(m_glVertexData + vPos, (GLbyte*)curVertices.data(), nBytes);
            vPos += nBytes;

            if (useIndices) {
                if (vertexOffset + nVertices > MAX_INDEX_VALUE) {
                    m_vertexOffsets.emplace_back(indexOffset, vertexOffset);
                    vertexOffset = 0;
                    indexOffset = 0;
                }

                for (int idx : indices[i]) {
                    m_glIndexData[iPos++] = idx + vertexOffset;
                }
                indexOffset += indices[i].size();
            }
            vertexOffset += nVertices;
        }

        m_vertexOffsets.emplace_back(indexOffset, vertexOffset);

        m_isCompiled = true;
    }
};

} // OGLW
