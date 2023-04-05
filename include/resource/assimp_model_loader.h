#include "graphics/model.h"

#include <assimp/Importer.hpp>

#include <resource/model_loader.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>

namespace arch {

class AssimpModelLoadingException : public ModelLoadingException {
public:
    AssimpModelLoadingException(Assimp::Importer &importer) : ModelLoadingException(importer.GetErrorString()) {}
};

class AssimpModelLoader : public ModelLoader {
public:
    Model load_model(const std::string &path) override;

private:
    const aiScene *read_scene(const std::string &path);
    Model extract_model(const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh);
    std::vector<Vertex> process_vertices(aiMesh *mesh);
    std::vector<Index> process_indices(aiMesh *mesh);

    static glm::vec3 to_glm_vec3(aiVector3D vec) {
        return glm::vec3 { vec.x, vec.y, vec.z };
    }

    static glm::vec2 to_glm_vec2(aiVector3D vec) {
        return glm::vec2 { vec.x, vec.y };
    }
};

}