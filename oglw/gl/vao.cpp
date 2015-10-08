#include "vao.h"
#include "core/log.h"

namespace OGLW {

Vao::Vao() {
    m_glVertexArray = nullptr;
    m_glnVAOs = 0;
}

Vao::~Vao() {
    if (m_glVertexArray) {
        GL_CHECK(glDeleteVertexArrays(m_glnVAOs, m_glVertexArray));
        delete[] m_glVertexArray;
    }
}

void Vao::init(GLuint _vertexBuffer, GLuint _indexBuffer, VertexLayout& _layout,
        const std::unordered_map<std::string, GLuint>& _locations, std::vector<size_t> _offsets) {

    m_glnVAOs = _offsets.size();

    m_glVertexArray = new GLuint[m_glnVAOs];

    GL_CHECK(glGenVertexArrays(m_glnVAOs, m_glVertexArray));

    for (int i = 0; i < _offsets.size(); ++i) {
        size_t offset = _offsets[i];

        // Bind the vertex array for initialization
        bind(i);

        // Bind the vertex buffer
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer));

        _layout.enable(_locations, offset);
    }

    // Make sure VAO is not modified outside
    unbind();
}

void Vao::bind(uint _vaoIndex) const {
    if (_vaoIndex < m_glnVAOs) {
        GL_CHECK(glBindVertexArray(m_glVertexArray[_vaoIndex]));
    } else {
        WARN("Trying to bind out of range vao index %d\n", _vaoIndex);
    }
}

void Vao::unbind() const {
	GL_CHECK(glBindVertexArray(0));
}

}
