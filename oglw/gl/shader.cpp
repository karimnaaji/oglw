#include "shader.h"

namespace OGLW {

Shader::Shader(std::string _fragPath, std::string _vertPath) {
    std::string vert, frag;

    if (!stringFromPath(_vertPath, &vert)) {
        std::cerr << "Can't load shader " << _vertPath << std::endl;
    }

    if (!stringFromPath(_fragPath, &frag)) {
        std::cerr << "Can't load shader " << _fragPath << std::endl;
    }

    if (!load(frag, vert)) {
        std::cerr << "Error building shader" << std::endl;
    }
}

Shader::~Shader() {
    GL_CHECK(glDeleteProgram(m_program));
}

bool Shader::load(const std::string& _fragmentSrc, const std::string& _vertexSrc) {
    m_vertexShader = compile(_vertexSrc, GL_VERTEX_SHADER);

    if (!m_vertexShader) {
        GL_CHECK(glDeleteShader(m_vertexShader));
        std::cerr << "Error loading vertex shader src" << std::endl;
        return false;
    }

    m_fragmentShader = compile(_fragmentSrc, GL_FRAGMENT_SHADER);

    if (!m_fragmentShader) {
        std::cerr << "Error loading fragment shader src" << std::endl;
        GL_CHECK(glDeleteShader(m_fragmentShader));
        return false;
    }

    m_program = GL_CHECK(glCreateProgram());

    GL_CHECK(glAttachShader(m_program, m_vertexShader));
    GL_CHECK(glAttachShader(m_program, m_fragmentShader));
    GL_CHECK(glLinkProgram(m_program));

    GLint isLinked;
    GL_CHECK(glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked));

    if (isLinked == GL_FALSE) {
        GLint infoLength = 0;
        GL_CHECK(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLength));

        if (infoLength > 1) {
            std::vector<GLchar> infoLog(infoLength);
            GL_CHECK(glGetProgramInfoLog(m_program, infoLength, NULL, &infoLog[0]));
            std::string error(infoLog.begin(), infoLog.end());
            std::cerr << "Error linking shader: " << error << std::endl;
        }

        GL_CHECK(glDeleteProgram(m_program));
        return false;
    } else {
        std::cout << "Shader linked successfuly" << std::endl;
        GL_CHECK(glDeleteShader(m_vertexShader));
        GL_CHECK(glDeleteShader(m_fragmentShader));
        return true;
    }
}

const GLint Shader::getAttribLocation(const std::string& _attribute) const {
    return GL_CHECK(glGetAttribLocation(m_program, _attribute.c_str()));
}

void Shader::use() const {
    if (!isInUse()) {
        GL_CHECK(glUseProgram(getProgram()));
    }
}

bool Shader::isInUse() const {
    GLint currentProgram = 0;
    GL_CHECK(glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram));
    return (GL_CHECK(getProgram()) == (GLuint)currentProgram);
}

GLuint Shader::compile(const std::string& _src, GLenum _type) {
    GLuint shader = glCreateShader(_type);
    const GLchar* source = (const GLchar*)_src.c_str();

    GL_CHECK(glShaderSource(shader, 1, &source, NULL));
    GL_CHECK(glCompileShader(shader));

    GLint isCompiled;
    GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));

    if (isCompiled == GL_FALSE) {
        GLint infoLength = 0;
        GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength));
        if (infoLength > 1) {
            std::vector<GLchar> infoLog(infoLength);
            GL_CHECK(glGetShaderInfoLog(shader, infoLength, NULL, &infoLog[0]));
            std::cerr << "Error compiling shader: " << std::endl;
            std::cerr << &infoLog[0] << std::endl;
        }
        GL_CHECK(glDeleteShader(shader));
        return 0;
    }

    return shader;
}

GLint Shader::getUniformLocation(const std::string& _uniformName) const {
    GLint loc = GL_CHECK(glGetUniformLocation(m_program, _uniformName.c_str()));
    if (loc == -1) {
        std::cerr << "Uniform " << _uniformName << " not found" << std::endl;
    }
    return loc;
}

void Shader::setUniform(const std::string& _name, float _x) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniform1f(location, _x));
    }
}

void Shader::setUniform(const std::string& _name, float _x, float _y) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniform2f(location, _x, _y));
    }
}

void Shader::setUniform(const std::string& _name, float _x, float _y, float _z) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniform3f(location, _x, _y, _z));
    }
}

void Shader::setUniform(const std::string& _name, float _x, float _y, float _z, float _w) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniform4f(location, _x, _y, _z, _w));
    }
}

void Shader::setUniform(const std::string& _name, const glm::mat2& _value, bool _transpose) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniformMatrix2fv(location, 1, _transpose, &_value[0][0]));
    }
}

void Shader::setUniform(const std::string& _name, const glm::mat3& _value, bool _transpose) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniformMatrix3fv(location, 1, _transpose, &_value[0][0]));
    }
}

void Shader::setUniform(const std::string& _name, const glm::mat4& _value, bool _transpose) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniformMatrix4fv(location, 1, _transpose, &_value[0][0]));
    }
}

}
