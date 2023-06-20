#include "graphics/renderer.h"

#include <spdlog/spdlog.h>

namespace arch {


Renderer::Renderer() {
    glEnable(GL_DEBUG_OUTPUT);
}

void GLAPIENTRY Renderer::_debug_message_callback(GLenum source, GLenum type,
												  GLuint id, GLenum severity,
												  GLsizei length,
												  const GLchar *message,
												  const void *user_param) {
   
}

void Renderer3D::render() {
    _shader.activate();
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
