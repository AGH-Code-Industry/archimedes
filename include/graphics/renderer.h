#include "graphics/model.h"
#include "graphics/shader.h"
#include "graphics/vertex_storage.h"

#include <vector>

namespace arch {

class RenderObject {
public:
    RenderObject(const Model &model);

    void render() const;

private:
    VertexStorage _vertex_storage;

};
    
/*
  * Base class for renderers that contains functionality common to 3D and 2D rendering.
  */
class Renderer {
protected:
    Renderer();
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
                
};

class Renderer3D : public Renderer {
public:
    void render();
    void submit(const Model &model);

private:
    Shader _shader;
    std::vector<RenderObject> _objs;
};


}
