#include "graphics/model.h"

namespace arch {
    
/*
  * Base class for renderers that contains functionality common to 3D and 2D rendering.
  */
class Renderer {};

class Renderer3D : public Renderer {
public:
    void submit(const Model &model);
};

}
