#include "vao.h"

namespace OGLW {

Vao::Vao() {
	m_glVertexArray = 0;
}

Vao::~Vao() {
    if (m_glVertexArray) {
        GL_CHECK(glDeleteVertexArrays(1, &m_glVertexArray));
    }
}

void Vao::init(GLuint _vertexBuffer, VertexLayout& _layout, const std::unordered_map<std::string, GLuint>& _locations, size_t _offset) {
    if (m_glVertexArray == 0) {
        GL_CHECK(glGenVertexArrays(1, &m_glVertexArray));
    }

    // Bind the vertex array for initialization
    bind();

    // Bind the vertex buffer
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
    
    // TODO : keep layout, test if other layout was enabled

    _layout.enable(_locations, _offset);

    // Make sure VAO is not modified outside
    unbind();
}

void Vao::bind() const {
    GL_CHECK(glBindVertexArray(m_glVertexArray));
}

void Vao::unbind() const {
	GL_CHECK(glBindVertexArray(0));
}

}