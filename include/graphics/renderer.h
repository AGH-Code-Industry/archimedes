#include "graphics/model.h"
#include "graphics/shader.h"
#include "graphics/vertex_storage.h"

#include <spdlog/spdlog.h>

#include <vector>

namespace arch {

class RenderObject {
public:
    RenderObject(const Model &model);

    void render() const;

private:
    VertexStorage _vertex_storage;

};

class OpenGLDebugMessagesHandler {
public:
    static void init();
private:
    static void GLAPIENTRY
    _debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *user_param
    );
    static spdlog::level::level_enum _determine_log_level(GLenum type, GLenum severity);

    static constexpr std::array<GLenum, 10> _warning_message_types {
        GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,  GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
        GL_DEBUG_TYPE_PORTABILITY,  GL_DEBUG_TYPE_PERFORMANCE
    };
};
/*
  * Base class for renderers that contains functionality common to 3D and 2D rendering.
  */
class Renderer {
protected:
    Renderer();
};

class Renderer3D : public Renderer {
public:
    void render();
    void submit(const Model &model);
    //TODO: Delet that. Only temporary
    void set_texture(Texture &&texture) { _texture = std::move(texture); }

private:
    Shader _shader;
    std::vector<RenderObject> _objs;
    Texture _texture;
};


}
