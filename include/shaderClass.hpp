//
// Created by Jakub on 27.12.2022.
//

#ifndef PROJEKT_SHADERCLASS_HPP
#define PROJEKT_SHADERCLASS_HPP

#include"glad/glad.h"
#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

/**
 * @brief Class for loading and using shaders
 * @details This class is used for loading and using shaders. It is used in the main class.
 */
class Shader {
public:
    /**
     * @brief Constructor of the shader class
     * @param vertexPath path to the vertex shader
     * @param fragmentPath path to the fragment shader
     * @param geometryPath path to the geometry shader
     */
    Shader(const std::string  &vertexPath, const std::string  &fragmentPath, const std::string  &geometryPath);
    /**
     * @brief Constructor of the shader class
     * @param vertexPath path to the vertex shader
     * @param fragmentPath path to the fragment shader
     */
    Shader(const std::string  &vertexPath, const std::string  &fragmentPath);


    Shader(const Shader& shader);

    Shader(const Shader&& shader) noexcept;

    Shader & operator=(const Shader& shader);

    virtual ~Shader();
    /**
    * @breif Sets uniform matrix
    * @param name
    * @param matrix
    */
    void setUniformMatrix4fv(const std::string &name, glm::mat4 matrix) const;
    /**
     * @brief Sets uniform float
     * @param name
     * @param value
     */
    void setUniform1f(const std::string &name, GLfloat value) const;
    /**
     * @brief Sets uniform integer
     * @param name
     * @param value
     */
    void setUniform1ui(const std::string &name, GLuint value) const;
    /**
     * @brief Sets uniform vector
     * @param name
     * @param vector
     */
    void setUniform1i(const std::string &name, GLint value) const;
    /**
     * Getter for the ID of the shader program
     * @return
     */
    GLuint getID() const;
    /**
     * @brief Activates the shader
     */
    void activate() const;
    /**
     * @brief Destructor of the shader class that uses the Delete() function
     */


private:
    /**
     * @brief ID of the shader program
     */
    GLuint ID;
    /**
     * @brief Compiles the shader
     * @param shaderCode code of the shader
     * @param shaderType type of the shader
     * @return ID of the shader
     */
    static GLuint compileShader(const char* shaderCode, GLenum shaderType);
    /**
     * @brief Checks if the shader was compiled successfully
     * @param shader ID of the shader
     * @param type type of the shader
     */
    static void checkCompileErrors(GLuint shader, const std::string& type);
    /**
     * @brief Gets the content of the shader file
     * @param path path to the shader file
     * @return content of the shader file
     */
    static std::string getFileContent(const std::string &path);
    /**
     * @brief Links the shaders to the program together
     * @param shaders array of shaders
     * @param count number of shaders
     */
    void linkProgram(GLuint *shaders, int count);



};
#endif //PROJEKT_SHADERCLASS_HPP
