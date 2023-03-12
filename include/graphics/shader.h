#ifndef SHADER_H
#define SHADER_H

#include"glad/glad.h"
#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<stdexcept>
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

    Shader & operator=(const Shader& shader) = delete;

    Shader & operator=(const Shader&& shader) noexcept = delete;

    ~Shader();
    /**
    * @breif Sets uniform matrix
    * @param name
    * @param matrix
    */
    void set_uniform_matrix_4_fv(const std::string &name, glm::mat4 matrix) const;
    /**
     * @brief Sets uniform float
     * @param name
     * @param value
     */
    void set_uniform_1_f(const std::string &name, GLfloat value) const;
    /**
     * @brief Sets uniform integer
     * @param name
     * @param value
     */
    void set_uniform_1_ui(const std::string &name, GLuint value) const;
    /**
     * @brief Sets uniform vector
     * @param name
     * @param vector
     */
    void set_uniform_1_i(const std::string &name, GLint value) const;
    /**
     * Getter for the ID of the shader program
     * @return
     */
    GLuint get_id() const;
    /**
     * @brief Activates the shader
     */
    void activate() const;

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
    static GLuint compile_shader(const char* shaderCode, GLenum shaderType);
    /**
     * @brief Checks if the shader was compiled successfully
     * @param shader ID of the shader
     * @param type type of the shader
     */
    static void check_compile_errors(GLuint shader, const std::string& type);
    /**
     * @brief Gets the content of the shader file
     * @param path path to the shader file
     * @return content of the shader file
     */
    static std::string get_file_content(const std::string &path);
    /**
     * @brief Links the shaders to the program together
     * @param shaders array of shaders
     * @param count number of shaders
     */
    void link_program(const std::vector<GLuint>& shaders);



};

class ShaderException : public std::runtime_error {
public:
    explicit ShaderException(const std::string &msg) : std::runtime_error(msg) {}
    const char * what () const noexcept override;
    ~ShaderException() override = default;
};
class ShaderCompileException : public ShaderException {
public:
    explicit ShaderCompileException(const std::string &msg) : ShaderException(msg) {}
    const char * what () const noexcept override;
};
class ShaderLinkException : public ShaderException {
public:
    explicit ShaderLinkException(const std::string &msg) : ShaderException(msg) {}
    const char * what () const noexcept override;
};
class ShaderFileLoadException : public ShaderException {
public:
    explicit ShaderFileLoadException(const std::string &msg) : ShaderException(msg) {}
    const char * what () const noexcept override;
};
#endif //SHADER_H