
#include "shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath) : ID(0) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    vertexCode = get_file_content(vertexPath);
    fragmentCode = get_file_content(fragmentPath);
    geometryCode = get_file_content(geometryPath);

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    GLuint vertex, fragment, geometry;

    vertex = compile_shader(vShaderCode, GL_VERTEX_SHADER);
    fragment = compile_shader(fShaderCode, GL_FRAGMENT_SHADER);
    geometry = compile_shader(gShaderCode, GL_GEOMETRY_SHADER);

    std::vector<GLuint> shaders = {vertex, fragment, geometry};

    link_program(shaders);

}
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) : ID(0) {
    std::string vertexCode;
    std::string fragmentCode;
    vertexCode = get_file_content(vertexPath);
    fragmentCode = get_file_content(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;

    vertex = compile_shader(vShaderCode, GL_VERTEX_SHADER);
    fragment = compile_shader(fShaderCode, GL_FRAGMENT_SHADER);

    std::vector<GLuint> shaders = {vertex, fragment};

    link_program(shaders);
}
Shader::Shader(const Shader &shader) {
    glDeleteProgram(ID);
    ID = shader.ID;
}
Shader::Shader(const Shader &&shader) noexcept {
    glDeleteProgram(ID);
    ID = shader.ID;
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::set_uniform_matrix_4_fv(const std::string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::set_uniform_1_i(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set_uniform_1_f(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set_uniform_1_ui(const std::string &name, GLuint value) const {
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
GLuint Shader::get_id() const{
    return ID;
}
void Shader::activate() const {
    glUseProgram(ID);
}

GLuint Shader::compile_shader(const char *shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    if (shaderType == GL_VERTEX_SHADER) {
        check_compile_errors(shader, "VERTEX");
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        check_compile_errors(shader, "FRAGMENT");
    } else {
        check_compile_errors(shader, "GEOMETRY");
    }
    return shader;
}


void Shader::check_compile_errors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            throw ShaderLinkException("Shader link error:" + type + '\n' + infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            throw ShaderCompileException("Shader compilation error:" + type  + '\n' + infoLog);
        }
    }
}

std::string Shader::get_file_content(const std::string &path) {
    std::string content;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        content = shaderStream.str();
    } catch (const std::ifstream::failure &e) {
        throw ShaderFileLoadException("Shader file load error: " + path);
    }
    return content;
}

void Shader::link_program(const std::vector<GLuint>& shaders) {
    ID = glCreateProgram();
    for(GLuint shader : shaders)
        glAttachShader(ID, shader);
    glLinkProgram(ID);
    check_compile_errors(ID, "PROGRAM");
    for(GLuint shader : shaders)
        glDeleteShader(shader);
}


const char *ShaderException::what() const noexcept {
    return runtime_error::what();
}

const char *ShaderCompileException::what() const noexcept {
    return ShaderException::what();
}

const char *ShaderLinkException::what() const noexcept {
    return ShaderException::what();
}

const char *ShaderFileLoadException::what() const noexcept {
    return ShaderException::what();
}
