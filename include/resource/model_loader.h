// #include "exception.h"
//
// #include <assimp/Importer.hpp>
// #include <resource/resource_loader.h>
// #include <assimp/scene.h>
// #include <assimp/vector3.h>
// #include <assimp/LogStream.hpp>
// #include <assimp/Importer.hpp>
//
// namespace arch {
//
// class SpdlogStream : public Assimp::LogStream {
// public:
//     void write(const char *msg) override;
// };
//
// class AssimpInitializer {
// public:
//     static void init();
// private:
//     static void init_logger();
// };
//
// class AssimpImportException : public Exception {
// public:
//     AssimpImportException(Assimp::Importer &importer) : Exception("Import file with assimp") {
//         append_msg(importer.GetErrorString());
//     }
// };
//
//
// class AssimpModelLoader : public ResourceLoader<Model> {
// public:
//     Model read_file(const std::filesystem::path &path) override;
//
// private:
//     const aiScene *read_scene(const std::string &path);
//     Model extract_model(const aiScene *scene);
//     Mesh process_mesh(aiMesh *mesh);
//     std::vector<Vertex> process_vertices(aiMesh *mesh);
//     std::vector<uint32_t> process_indices(aiMesh *mesh);
//
//     static glm::vec3 to_glm_vec3(aiVector3D vec) {
//         return glm::vec3 { vec.x, vec.y, vec.z };
//     }
//
//     static glm::vec2 to_glm_vec2(aiVector3D vec) {
//         return glm::vec2 { vec.x, vec.y };
//     }
// };
//
// }