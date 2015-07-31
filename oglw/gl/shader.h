#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "gl.h"
#include "glm/glm.hpp"

namespace OGLW {

class Shader {

public:
    Shader(std::string _fragPath, std::string _vertPath);
    ~Shader();

    const GLuint getProgram() const {
        return m_program;
    };
    const GLuint getFragmentShader() const {
        return m_fragmentShader;
    };
    const GLuint getVertexShader() const {
        return m_vertexShader;
    };

    void use() const;
    bool isInUse() const;

    const GLint getAttribLocation(const std::string& _attribute) const;
    bool load(const std::string& _fragmentSrc, const std::string& _vertexSrc);

    void setUniform(const std::string& _name, float _x);
    void setUniform(const std::string& _name, float _x, float _y);
    void setUniform(const std::string& _name, float _x, float _y, float _z);
    void setUniform(const std::string& _name, float _x, float _y, float _z, float _w);
    void setUniform(const std::string& _name, const glm::vec2& _value) {
        setUniform(_name, _value.x, _value.y);
    }
    void setUniform(const std::string& _name, const glm::vec3& _value) {
        setUniform(_name, _value.x, _value.y, _value.z);
    }
    void setUniform(const std::string& _name, const glm::vec4& _value) {
        setUniform(_name, _value.x, _value.y, _value.z, _value.w);
    }
    void setUniform(const std::string& _name, const glm::mat2& _value, bool transpose = false);
    void setUniform(const std::string& _name, const glm::mat3& _value, bool transpose = false);
    void setUniform(const std::string& _name, const glm::mat4& _value, bool transpose = false);

private:
    GLuint compile(const std::string& _src, GLenum _type);
    GLint getUniformLocation(const std::string& _uniformName) const;

    GLuint m_program;
    GLuint m_fragmentShader;
    GLuint m_vertexShader;
};

} // OGLW
