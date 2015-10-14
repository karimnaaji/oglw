#pragma once

#include "gl.h"
#include "vertexLayout.h"
#include "types.h"
#include <unordered_map>
#include <vector>

namespace OGLW {

class Vao {
public:
	Vao();
	~Vao();

	// init a vertex layout
	void init(GLuint _vertexBuffer, GLuint _indexBuffer, VertexLayout& _layout,
        const std::unordered_map<std::string, GLuint>& _locations);
	// bind the vertex array object
	void bind() const;
	// usefull fot the VAO not to be modified outside
	void unbind() const;

private:
    GLuint m_glVertexArray;

};

}
