#pragma once

#include <vector>
#include <memory>
#include <cstring>
#include <unordered_map>
#include "gl/glTypes.h"
#include "gl/vertexLayout.h"
#include "gl/shader.h"
#include "gl/vao.h"

namespace OGLW {

#define MAX_INDEX_VALUE 65535

class VboMesh {

public:
    VboMesh(std::shared_ptr<VertexLayout> _vertexlayout, GLenum _drawMode = GL_TRIANGLES, GLenum _hint = GL_STATIC_DRAW);
    VboMesh();
    virtual ~VboMesh();

    void setVertexLayout(std::shared_ptr<VertexLayout> _vertexLayout);
    void setDrawMode(GLenum _drawMode = GL_TRIANGLES);

    // number of vertices in the mesh
    int numVertices() const { return m_nVertices; }
    // number of indices in the mesh
    int numIndices() const { return m_nIndices; }
    // compile the vertex buffer to unsigned byte data for ready for upload
    virtual void compileVertexBuffer() = 0;
    // draw the mesh for a specific shader program
    void draw(Shader& _shader);
    // compute normals for a set of vertices and indices
    static std::vector<glm::vec3> computeNormals(std::vector<glm::vec3> _vertices, std::vector<int> _indices);
    // get the buffer dirty size (data not yet uploaded in gpu)
    GLsizei getDirtySize() const { return m_dirtySize; }
    // get the buffer dirty offset (memory location offset not yet uploaded starts)
    GLintptr getDirtyOffset() const { return m_dirtyOffset; }

protected:
    bool upload();
    bool subDataUpload();

    std::vector<std::pair<uint32_t, uint32_t>> m_vertexOffsets;
    std::shared_ptr<VertexLayout> m_vertexLayout;

    int m_nVertices;
    GLuint m_glVertexBuffer;
    GLbyte* m_glVertexData = nullptr;
    std::unique_ptr<Vao> m_vao;

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
