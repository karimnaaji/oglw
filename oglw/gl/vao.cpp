#include "vao.h"
#include "core/log.h"

namespace OGLW {

Vao::Vao() {
    m_glVertexArray = 0;
}

Vao::~Vao() {
    if (m_glVertexArray) {
        GL_CHECK(glDeleteVertexArrays(1, &m_glVertexArray));
    }
}

void Vao::generate() {
    if (!m_glVertexArray) {
        GL_CHECK(glGenVertexArrays(1, &m_glVertexArray));
    } else {
        WARN("VAO already generated\n");
    }
}

void Vao::init(GLuint _vertexBuffer, GLuint _indexBuffer,
    VertexLayout& _layout,
    const std::unordered_map<std::string, GLuint>&
    _locations)
{
    generate();

    // Bind the vertex array for initialization
    bind();

    // Bind the vertex and index buffer
    if (_vertexBuffer) {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
    }

    if (_indexBuffer) {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer));
    }

    _layout.enable(_locations);

    // Make sure VAO is not modified outside
    unbind();
}

void Vao::bind() const {
    GL_CHECK(glBindVertexArray(m_glVertexArray));
}

void Vao::unbind() const {
	GL_CHECK(glBindVertexArray(0));
}

} // OGLW
