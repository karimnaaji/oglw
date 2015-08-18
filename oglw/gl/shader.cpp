#include "shader.h"
#include "core/utils.h"
#include "core/log.h"

namespace OGLW {

Shader::Shader(std::string _fragPath, std::string _vertPath, std::string _geomPath) {
    std::string vert, frag, geom;
    stringFromPath(_vertPath, &vert);
    stringFromPath(_fragPath, &frag);

    if (_geomPath != "") {
        stringFromPath(_geomPath, &geom);
    }

    if (!load(frag, vert)) {
        WARN("Failed to build shader %s %s", _fragPath.c_str(), _vertPath.c_str());
    }
}

Shader::Shader(std::string _programBundlePath) {
    std::string vert, frag, bundle;
    stringFromPath(_programBundlePath, &bundle);

    if (getBundleShaderSource("vertex", bundle, &vert) && 
        getBundleShaderSource("fragment", bundle, &frag)) {
        if (!load(frag, vert)) {
            WARN("Failed to build shader program bundle %s", _programBundlePath.c_str());
        }
    } else {
        WARN("Failed to load shader in program bundle %s", _programBundlePath.c_str());
    }
}

Shader::~Shader() {
    GL_CHECK(glDeleteProgram(m_program));
}

bool Shader::getBundleShaderSource(std::string _type, std::string _bundle, std::string* _out) const {
    const std::string startTag = "#pragma begin:" + _type;
    const std::string endTag = "#pragma end:" + _type;

    size_t start = _bundle.find(startTag);
    start += startTag.length();
    size_t end = _bundle.find(endTag);

    if (start != std::string::npos && end != std::string::npos) {
        *_out = _bundle.substr(start, end);
        return true;
    }

    return false;
}

GLuint Shader::add(const std::string& _shaderSource, GLenum _kind) {
  GLuint shader = compile(_shaderSource, _kind);

    if (!shader) {
        GL_CHECK(glDeleteShader(shader));
        WARN("Failed to compile shader");
        WARN("%s", _shaderSource.c_str());
        return -1;
    }

    GL_CHECK(glAttachShader(m_program, shader));

    return shader;
}

bool Shader::load(const std::string& _fragmentSrc, const std::string& _vertexSrc) {
    m_program = glCreateProgram();
    GL_CHECK(void(0));

    // add vertex shader to shader program
    GLuint vert = add(_vertexSrc, GL_VERTEX_SHADER);
    // add fragment shader to shader program
    GLuint frag = add(_fragmentSrc, GL_FRAGMENT_SHADER);

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
            DBG("Error linking shader program");
            DBG(error.c_str());
        }

        GL_CHECK(glDeleteProgram(m_program));
        return false;
    } else {
        GL_CHECK(glDeleteShader(vert));
        GL_CHECK(glDeleteShader(frag));
        return true;
    }
}

GLint Shader::getAttribLocation(const std::string& _attribute) const {
    GLint attribute = glGetAttribLocation(m_program, _attribute.c_str());
    GL_CHECK(void(0));
    return attribute;
}

void Shader::use() const {
    if (!isInUse()) {
        GL_CHECK(glUseProgram(getProgram()));
    }
}

bool Shader::isInUse() const {
    GLint currentProgram = 0;
    GL_CHECK(glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram));
    return getProgram() == (GLuint)currentProgram;
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
            DBG("Compilation error");
            DBG(_src.c_str());
            DBG(&infoLog[0]);
        }
        GL_CHECK(glDeleteShader(shader));
        return 0;
    }

    return shader;
}

GLint Shader::getUniformLocation(const std::string& _uniformName) const {
    GLint loc = glGetUniformLocation(m_program, _uniformName.c_str());
    GL_CHECK(void(0));
    if (loc == -1) {
        WARN("shader uniform %s not found on shader program: %d", m_program);
    }
    return loc;
}


void Shader::setUniform(const std::string& _name, int _x) {
    use();
    GLint location = getUniformLocation(_name);
    if (location >= 0) {
        GL_CHECK(glUniform1i(location, _x));
    }
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

void Shader::setUniform(const std::string& _name, const glm::vec2& _value) {
    setUniform(_name, _value.x, _value.y);
}

void Shader::setUniform(const std::string& _name, const glm::vec3& _value) {
    setUniform(_name, _value.x, _value.y, _value.z);
}

void Shader::setUniform(const std::string& _name, const glm::vec4& _value) {
    setUniform(_name, _value.x, _value.y, _value.z, _value.w);
}

GLuint Shader::getProgram() const {
    return m_program;
}

GLuint Shader::getFragmentShader() const {
    return m_fragmentShader;
}

GLuint Shader::getVertexShader() const {
    return m_vertexShader;
}

} // OGLW
