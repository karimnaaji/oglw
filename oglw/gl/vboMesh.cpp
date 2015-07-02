#include "vboMesh.h"

namespace OGLW {

    VboMesh::VboMesh(std::shared_ptr<VertexLayout> _vertexLayout, GLenum _drawMode) : VboMesh() {
        m_vertexLayout = _vertexLayout;
        setDrawMode(_drawMode);
    }

    VboMesh::VboMesh() {
        m_glVertexBuffer = 0;
        m_glIndexBuffer = 0;
        m_glVertexArray = 0;
        m_nVertices = 0;
        m_nIndices = 0;
        m_isUploaded = false;
        m_isCompiled = false;
    }

    VboMesh::~VboMesh() {
        if (m_glVertexBuffer) {
            glDeleteBuffers(1, &m_glVertexBuffer);
        }

        if (m_glIndexBuffer) {
            glDeleteBuffers(1, &m_glIndexBuffer);
        }

        if (m_glVertexArray) {
            glDeleteVertexArrays(1, &m_glVertexArray);
        }
    }

    void VboMesh::setVertexLayout(std::shared_ptr<VertexLayout> _vertexLayout) {
        m_vertexLayout = _vertexLayout;
    }

    void VboMesh::setDrawMode(GLenum _drawMode) {
        switch (_drawMode) {
        case GL_POINTS:
        case GL_LINE_STRIP:
        case GL_LINE_LOOP:
        case GL_LINES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
        case GL_TRIANGLES:
            m_drawMode = _drawMode;
            break;
        default:
            m_drawMode = GL_TRIANGLES;
        }
    }

    void VboMesh::upload() {
        if (m_glVertexBuffer == 0) {
            glGenBuffers(1, &m_glVertexBuffer);
        }

        if (m_glVertexArray == 0) {
            glGenVertexArrays(1, &m_glVertexArray);
        }

        if (!m_isCompiled) {
            compileVertexBuffer();
        }

        int vertexBytes = m_nVertices * m_vertexLayout->getStride();

        glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexBytes, m_glVertexData, GL_STATIC_DRAW);

        if (m_glIndexData) {

            if (m_glIndexBuffer == 0) {
                glGenBuffers(1, &m_glIndexBuffer);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(GLushort), m_glIndexData, GL_STATIC_DRAW);

            delete[] m_glIndexData;
        }

        delete[] m_glVertexData;

        m_isUploaded = true;
    }

    void VboMesh::draw(const Shader& _shader) {
        if (!m_isUploaded) {
            upload();
        }

        glBindVertexArray(m_glVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);

        if (m_nIndices > 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
        }

        _shader.use();

        size_t indiceOffset = 0;
        size_t vertexOffset = 0;

        for (auto& o : m_vertexOffsets) {
            uint32_t nIndices = o.first;
            uint32_t nVertices = o.second;

            size_t byteOffset = vertexOffset * m_vertexLayout->getStride();

            m_vertexLayout->enable(_shader, byteOffset);

            if (nIndices > 0) {
                glDrawElements(m_drawMode, nIndices, GL_UNSIGNED_SHORT, (void*)(indiceOffset * sizeof(GLushort)));
            } else if (nVertices > 0) {
                glDrawArrays(m_drawMode, 0, nVertices);
            }

            m_vertexLayout->disable(_shader);

            vertexOffset += nVertices;
            indiceOffset += nIndices;
        }
    }
}
