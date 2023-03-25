namespace arch {
    
//typedef unsigned int TextureId;
using TextureId = unsigned int;

class Texture {
public:
    Texture(Texture &other) = delete;
    Texture(Texture &&other) = delete;
    Texture& operator=(Texture &other) = delete;
    Texture& operator=(Texture &&other) = delete;
private:
    TextureId _id;
};

}