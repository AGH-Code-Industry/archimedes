#include "graphics/texture.h"

#include <glad/glad.h>
#include "graphics/primitives.h"

using namespace arch;

Texture::Texture(uint32_t width, uint32_t height, const void* data) {
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(Texture &&other) {
    _id = other._id;
    other._id = INVALID_OBJECT;
}

Texture& Texture::operator=(Texture &&other) {
    glDeleteTextures(1, &_id);
    _id = other._id;
    other._id = INVALID_OBJECT;
    return *this;
}

Texture::~Texture() {
    if (_id != INVALID_OBJECT)
        glDeleteTextures(1, &_id);
    _id = INVALID_OBJECT;
}
