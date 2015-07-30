#include "vertexLayout.h"

namespace OGLW {

VertexLayout::VertexLayout(std::vector<VertexAttrib> _attribs) : m_attribs(_attribs) {
    m_stride = 0;

    for (unsigned int i = 0; i < m_attribs.size(); i++) {
        m_attribs[i].offset = reinterpret_cast<void*>(m_stride);

        GLint byteSize = m_attribs[i].size;

        switch (m_attribs[i].type) {
        case GL_FLOAT:
        case GL_INT:
        case GL_UNSIGNED_INT:
            byteSize *= 4;
            break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            byteSize *= 2;
            break;
        }

        m_stride += byteSize;
    }
}

VertexLayout::~VertexLayout() {
    m_attribs.clear();
}

void VertexLayout::enable(const Shader& _program, size_t byteOffset) {

    for (auto& attrib : m_attribs) {
        const GLint location = _program.getAttribLocation(attrib.name);

        if (location != -1) {
            GL_CHECK(glEnableVertexAttribArray(location));
            GL_CHECK(glVertexAttribPointer(location, attrib.size, attrib.type, attrib.normalized, m_stride,
                                  ((unsigned char*)attrib.offset) + byteOffset));
        }
    }
}

void VertexLayout::disable(const Shader& _program) {
    for (auto& attrib : m_attribs) {

        const GLint location = _program.getAttribLocation(attrib.name);
        if (location != -1) {
            GL_CHECK(glDisableVertexAttribArray(location));
        }
    }
}

} // OGLW
