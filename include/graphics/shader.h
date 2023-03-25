#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
/**
 * @brief Class for loading and using shaders
 * @details This class is used for loading and using shaders. It is used in the main class.
 */
class Shader {
public:
    /**
     * @brief Constructor of the shader class
     * @param std::vector<std::string> paths to the shader files
     */
    Shader(const std::vector<std::string>& paths);

    /**
     * @brief Create default shader
     */
    Shader();
    Shader(const Shader& shader) = delete;
    Shader(const Shader&& shader) noexcept;
    Shader & operator=(const Shader& shader) = delete;
    Shader & operator=(const Shader&& shader) noexcept;

    ~Shader();
    /**
    * @breif Sets uniform matrix
    * @param name
    * @param matrix
    */
    void set_uniform_matrix4fv(const std::string &name, glm::mat4 matrix) const;
    /**
     * @brief Sets uniform float
     * @param name
     * @param value
     */
    void set_uniform1f(const std::string &name, GLfloat value) const;
    /**
     * @brief Sets uniform integer
     * @param name
     * @param value
     */
    void set_uniform1ui(const std::string &name, GLuint value) const;
    /**
     * @brief Sets uniform vector
     * @param name
     * @param vector
     */
    void set_uniform1i(const std::string &name, GLint value) const;
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
     */
    static void check_compile_errors(GLuint shader);
    /**
    * @brief Checks if the shader was linked successfully
    * @param program ID of the shader(program)
    */
    static void check_link_errors(GLuint program);
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

    const char *default_vertex_source = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main() {\n"
                                        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}";
    const char *default_fragment_source = "#version 330 core\n"
                                          "out vec4 FragColor;\n"
                                          "void main() {\n"
                                          "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                          "}";


};

class ShaderException : public std::runtime_error {
public:
    explicit ShaderException(const std::string &msg) : std::runtime_error(msg) {}
    const char * what () const noexcept override;
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
