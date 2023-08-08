#include "graphics/renderer.h"

#include <sstream>

namespace arch {

void arch::OpenGLDebugMessagesHandler::init() {
    //check if glad supports debug output
    if (!glDebugMessageCallback) {
        spdlog::warn("glDebugMessageCallback not available due to arch::OpenGLDebugMessagesHandler::init() function call.");
        return;
    }
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(_debug_message_callback, nullptr);    
}

void GLAPIENTRY OpenGLDebugMessagesHandler::_debug_message_callback(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *message, const void *user_param) 
{
    std::stringstream ss;
    ss << "OpenGLMessage (type " << type << ", severity " << severity << "): " 
       << message;
    auto level = _determine_log_level(type, severity);
    spdlog::log(level, ss.str());
}

spdlog::level::level_enum OpenGLDebugMessagesHandler::_determine_log_level(
	GLenum type, GLenum severity) 
{
    if (type == GL_DEBUG_TYPE_ERROR)
        return spdlog::level::level_enum::err;
    for (const auto& warning_type : _warning_message_types) {
        if (type == warning_type)
            return spdlog::level::level_enum::warn;
    }
    return spdlog::level::debug;
}

Renderer::Renderer() { OpenGLDebugMessagesHandler::init(); }

void Renderer3D::render() {
    _shader.activate();
    _texture.use();
    for (const auto &obj : _objs) {
        obj.render();
    }
}

void Renderer3D::submit(const Model &model) {
    _objs.emplace_back(model);
}

RenderObject::RenderObject(const Model &model) {
    const Mesh &mesh = model.meshes[0];
    _vertex_storage.set(mesh.vertices, mesh.indices);
}

void RenderObject::render() const {
    _vertex_storage.use();
    glDrawElements(GL_TRIANGLES, _vertex_storage.get_num_of_indices(), GL_UNSIGNED_INT, 0);
}

}
