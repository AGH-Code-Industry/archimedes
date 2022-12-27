//
// Created by Jakub on 27.12.2022.
//

#include "shaderClass.hpp"

GLuint Shader::compileShader(const char *shaderCode, GLenum shaderType) {
    // Build and compile our shader
    GLuint shader = glCreateShader(shaderType);
    // Attach our source code to the vertex shader
    glShaderSource(shader, 1, &shaderCode, nullptr);
    // Compile the vertex shader
    glCompileShader(shader);
    // Check for shader compile time errors
    if (shaderType == GL_VERTEX_SHADER) {
        checkCompileErrors(shader, "VERTEX");
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        checkCompileErrors(shader, "FRAGMENT");
    } else {
        checkCompileErrors(shader, "GEOMETRY");
    }
    return shader;
}


void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: \n" << infoLog << std::endl;
        }
    }
}

std::string Shader::getFileContent(const std::string &path) {
    std::string content;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        content = shaderStream.str();
    } catch (const std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path <<std::endl;
    }
    return content;
}

void Shader::linkProgram(GLuint *shaders, int count) {
    // Link shaders
    ID = glCreateProgram();
    for (int i = 0; i < count; i++) {
        glAttachShader(ID, shaders[i]);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessary
    for (int i = 0; i < count; i++) {
        glDeleteShader(shaders[i]);
    }
}


Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath) {
    ID = 0;//CLANG IS A CRYBABY
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    vertexCode = getFileContent(vertexPath);
    fragmentCode = getFileContent(fragmentPath);
    geometryCode = getFileContent(geometryPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    GLuint vertex, fragment, geometry;

    vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);
    geometry = compileShader(gShaderCode, GL_GEOMETRY_SHADER);

    linkProgram(new GLuint []{vertex, fragment, geometry} , 3);

}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    ID = 0;//CLANG IS A CRYBABY
    std::string vertexCode;
    std::string fragmentCode;
    vertexCode = getFileContent(vertexPath);
    fragmentCode = getFileContent(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;

    vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

    linkProgram(new GLuint []{vertex, fragment} , 2);

}

void Shader::setUniformMatrix4fv(const std::string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setUniform1i(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniform1f(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform1ui(const std::string &name, GLuint value) const {
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}

GLuint Shader::getID() const{
    return ID;
}

void Shader::Activate() const {
    glUseProgram(ID);
}
void Shader::Delete() {
    wasDeleted = true;
    glDeleteProgram(ID);
}

Shader::~Shader() {
    if (!wasDeleted) {
        Delete();
    }
}

