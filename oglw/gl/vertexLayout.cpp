#include "vertexLayout.h"
#include "core/types.h"
#include "gl/shader.h"
#include "core/log.h"

namespace OGLW {

VertexLayout::VertexLayout(std::vector<VertexAttrib> _attribs) : m_attribs(_attribs) {
    m_stride = 0;

    for (uint i = 0; i < m_attribs.size(); i++) {
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

std::unordered_map<std::string, GLuint> VertexLayout::getLocations() const {
    std::unordered_map<std::string, GLuint> map;

    for (auto& attribute : m_attribs) {
        if (attribute.location == -1) {
            WARN("Attribute location not set for attribute %s\n", attribute.name.c_str());
        }
        map[attribute.name] = attribute.location;
    }

    return map;
}

void VertexLayout::enable(const std::unordered_map<std::string, GLuint>& _locations,
    size_t _byteOffset,
    void* _ptr)
{
    for (auto& attrib : m_attribs) {
        auto it = _locations.find(attrib.name);

        if (it == _locations.end()) {
            continue;
        }

        const GLint location = it->second;

        void* data = _ptr ? _ptr : ((uchar*) attrib.offset) + _byteOffset;
        GL_CHECK(glEnableVertexAttribArray(location));
        GL_CHECK(glVertexAttribPointer(location,
                    attrib.size,
                    attrib.type,
                    attrib.normalized,
                    m_stride,
                    data));
    }
}

void VertexLayout::disable(const std::unordered_map<std::string, GLuint>& _locations) {
    for (auto& attrib : m_attribs) {
        auto it = _locations.find(attrib.name);

        if (it == _locations.end()) {
            continue;
        }
        const GLint location = it->second;
        GL_CHECK(glDisableVertexAttribArray(location));
    }
}

} // OGLW
