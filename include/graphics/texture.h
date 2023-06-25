#include "graphics/primitives.h"

namespace arch {
    
/**
 * A handle to 2-dimentional 32-bit RGBA image stored in VRAM.
*/
class Texture {
public:
    Texture() {}
    Texture(uint32_t width, uint32_t height, const void* data);
    ~Texture();
    Texture(Texture&) = delete;
    Texture(Texture &&other);
    Texture& operator=(Texture&) = delete;
    Texture& operator=(Texture &&other);
private:
    uint32_t _id = INVALID_OBJECT;
};

}