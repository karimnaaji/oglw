#include "vboMesh.h"
#include "gl.h"

namespace OGLW {

VboMesh::VboMesh(std::shared_ptr<VertexLayout> _vertexLayout, GLenum _drawMode, GLenum _hint) : VboMesh() {
    m_vertexLayout = _vertexLayout;
    m_hint = _hint;
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
    m_dirtyOffset = 0;
    m_dirtySize = 0;
    m_dirty = false;
}

VboMesh::~VboMesh() {
    if (m_glVertexBuffer) {
        GL_CHECK(glDeleteBuffers(1, &m_glVertexBuffer));
    }

    if (m_glIndexBuffer) {
        GL_CHECK(glDeleteBuffers(1, &m_glIndexBuffer));
    }

    if (m_glVertexArray) {
        GL_CHECK(glDeleteVertexArrays(1, &m_glVertexArray));
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

bool VboMesh::upload() {
    if (m_isUploaded) {
        return false;
    }

    if (m_glVertexBuffer == 0) {
        GL_CHECK(glGenBuffers(1, &m_glVertexBuffer));
    }

    if (m_glVertexArray == 0) {
        GL_CHECK(glGenVertexArrays(1, &m_glVertexArray));
    }

    if (!m_isCompiled) {
        compileVertexBuffer();
    }

    int vertexBytes = m_nVertices * m_vertexLayout->getStride();

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexBytes, m_glVertexData, GL_STATIC_DRAW));

    if (m_glIndexData) {

        if (m_glIndexBuffer == 0) {
            GL_CHECK(glGenBuffers(1, &m_glIndexBuffer));
        }

        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(GLushort), m_glIndexData, GL_STATIC_DRAW));

        delete[] m_glIndexData;
        m_glIndexData = nullptr;
    }

    delete[] m_glVertexData;
    m_glVertexData = nullptr;
    m_isUploaded = true;

    return true;
}

bool VboMesh::subDataUpload() {
    if (!m_dirty) {
        return false;
    }

    if (m_hint == GL_STATIC_DRAW) {
        WARN("wrong usage hint provided to the Vbo");
    }

    GL_CHECK(glBindVertexArray(m_glVertexArray));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));

    long vertexBytes = m_nVertices * m_vertexLayout->getStride();

    // when all vertices are modified, it's better to update the entire mesh
    if (vertexBytes - m_dirtySize < m_vertexLayout->getStride()) {

        // invalidate the data store on the driver
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexBytes, NULL, m_hint));

        // if this buffer is still used by gpu on current frame this call will not wait
        // for the frame to finish using the vbo but "directly" send command to upload the data
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexBytes, m_glVertexData, m_hint));
    } else {
        // perform simple sub data upload for part of the buffer
        GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, m_dirtyOffset, m_dirtySize, m_glVertexData + m_dirtyOffset));
    }

    m_dirtyOffset = 0;
    m_dirtySize = 0;

    m_dirty = false;

    // Also bind indices when return 'already bound'.
    if (m_nIndices > 0) {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer));
    }

    return true;
}

void VboMesh::draw(const Shader& _shader) {
    bool bound = false;

    if (!m_isUploaded) {
        bound = upload();
    } else if (m_dirty) {
        bound = subDataUpload();
    }

    if (!bound) {
        GL_CHECK(glBindVertexArray(m_glVertexArray));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));

        if (m_nIndices > 0) {
            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer));
        }
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
            GL_CHECK(glDrawElements(m_drawMode, nIndices, GL_UNSIGNED_SHORT, (void*)(indiceOffset * sizeof(GLushort))));
        } else if (nVertices > 0) {
            GL_CHECK(glDrawArrays(m_drawMode, 0, nVertices));
        }

        m_vertexLayout->disable(_shader);

        vertexOffset += nVertices;
        indiceOffset += nIndices;
    }
}

}
