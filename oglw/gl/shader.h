#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "gl.h"
#include "glm/glm.hpp"

namespace OGLW {

class Shader {

public:
    Shader(std::string _programBundlePath);
    Shader(std::string _fragPath, std::string _vertPath, std::string _geomPath = "");
    ~Shader();

    // get the GL shader program handle
    GLuint getProgram() const;
    // get the GL fragment shader handle
    GLuint getFragmentShader() const;
    // get the GL vertex shader handle
    GLuint getVertexShader() const;

    // sets the current program
    void use() const;
    // checks whether the shader program is in use
    bool isInUse() const;

    // retrieve the attribute location for a given attribute name
    GLint getAttribLocation(const std::string& _attribute) const;
    // set uniforms for the given name
    void setUniform(const std::string& _name, int _x);
    void setUniform(const std::string& _name, float _x);
    void setUniform(const std::string& _name, float _x, float _y);
    void setUniform(const std::string& _name, float _x, float _y, float _z);
    void setUniform(const std::string& _name, float _x, float _y, float _z, float _w);
    void setUniform(const std::string& _name, const glm::vec2& _value);
    void setUniform(const std::string& _name, const glm::vec3& _value);
    void setUniform(const std::string& _name, const glm::vec4& _value);
    void setUniform(const std::string& _name, const glm::mat2& _value, bool transpose = false);
    void setUniform(const std::string& _name, const glm::mat3& _value, bool transpose = false);
    void setUniform(const std::string& _name, const glm::mat4& _value, bool transpose = false);

private:
    // compile and attach a shader to the shader program
    GLuint add(const std::string& _shaderSource, GLenum _kind);
    // retrive a shader source from a program bundle
    bool getBundleShaderSource(std::string _type, std::string _bundle, std::string* _out) const;
    // load a shader program
    bool load(const std::string& _fragmentSrc, const std::string& _vertexSrc);
    // compile the shader program for the specified type
    GLuint compile(const std::string& _src, GLenum _type);
    // retrieve the uniform location for a given name
    GLint getUniformLocation(const std::string& _uniformName) const;

    // GL shader program handle
    GLuint m_program = -1;
    // GL fragment shader id
    GLuint m_fragmentShader = -1;
    // GL vertex shader id
    GLuint m_vertexShader = -1;
};

} // OGLW
