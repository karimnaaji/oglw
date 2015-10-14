#include "vboMesh.h"
#include "gl/gl.h"
#include "core/log.h"

namespace OGLW {

VboMesh::VboMesh(std::shared_ptr<VertexLayout> _vertexLayout, GLenum _drawMode, GLenum _hint) : VboMesh() {
    m_vertexLayout = _vertexLayout;
    m_hint = _hint;
    setDrawMode(_drawMode);
}

VboMesh::VboMesh() {
    m_glVertexBuffer = 0;
    m_glIndexBuffer = 0;
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

    // Create vertex Buffer if needed
    if (m_glVertexBuffer == 0) {
        GL_CHECK(glGenBuffers(1, &m_glVertexBuffer));
    }

    if (!m_isCompiled) {
        compileVertexBuffer();
    }

    int vertexBytes = m_nVertices * m_vertexLayout->getStride();

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexBytes, m_glVertexData, m_hint));

    if (m_glIndexData) {
        if (m_glIndexBuffer == 0) {
            GL_CHECK(glGenBuffers(1, &m_glIndexBuffer));
        }

        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(GLuint), m_glIndexData, GL_STATIC_DRAW));

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
        WARN("wrong usage hint provided to the Vbo\n");
    }

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

void VboMesh::draw(Shader& _shader) {
    if (!m_isUploaded) {
        upload();
    } else if (m_dirty) {
        subDataUpload();
    }

    // if VAO not yet created, initialized it and capture related states
    if (!m_vao) {
        m_vao = std::unique_ptr<Vao>(new Vao());
        const auto& locations = m_vertexLayout->getLocations();

        m_vao->init(m_glVertexBuffer, m_glIndexBuffer, *m_vertexLayout, locations);
    }

    _shader.bindVertexLayout(*m_vertexLayout);
    _shader.use();

    m_vao->bind();

    if (m_nIndices > 0) {
        GL_CHECK(glDrawElements(m_drawMode, m_nIndices, GL_UNSIGNED_INT, NULL));
    } else if (m_nVertices > 0) {
        GL_CHECK(glDrawArrays(m_drawMode, 0, m_nVertices));
    }

    m_vao->unbind();
}

std::vector<glm::vec3> VboMesh::computeNormals(std::vector<glm::vec3> _vertices, std::vector<int> _indices) {
    std::vector<glm::vec3> normals;
    normals.resize(_vertices.size());

    for (uint i = 0; i < _indices.size() / 3; ++i) {
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

} // OGLW
