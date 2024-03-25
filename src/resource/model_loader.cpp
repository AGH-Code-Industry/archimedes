// #include "resource/model_loader.h"
// #include "assimp/mesh.h"
// #include "gfx/model.h"
// #include "gfx/primitives.h"
//
// #include <assimp/postprocess.h>
// #include <spdlog/spdlog.h>
// #include <assimp/DefaultLogger.hpp>
// #include <assimp/Logger.hpp>
//
// namespace arch {
//
// void SpdlogStream::write(const char *msg) {
//     spdlog::debug("{}", msg);
// }
//
// void AssimpInitializer::init() {
//     init_logger();
// }
//
// void AssimpInitializer::init_logger() {
//     const unsigned int severity =
//         Assimp::Logger::Debugging | Assimp::Logger::Info |
//         Assimp::Logger::Err | Assimp::Logger::Warn;
//     Assimp::DefaultLogger::get()->attachStream(new SpdlogStream, severity);
// }
// Model AssimpModelLoader::read_file(const std::filesystem::path &path) {
//     return {};
// }
//
//
// const aiScene *AssimpModelLoader::read_scene(const std::string &path) {
//     Assimp::Importer importer {};
//     const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//     if (scene == nullptr) {
//         throw AssimpImportException {importer};
//     }
//     return scene;
// }
//
// Model AssimpModelLoader::extract_model(const aiScene *scene) {
//     std::vector<Mesh> meshes {};
//     for (int i = 0; i < scene->mNumMeshes; i++) {
//         meshes.push_back(process_mesh(scene->mMeshes[i]));
//     }
//     return Model { meshes };
// }
//
// Mesh AssimpModelLoader::process_mesh(aiMesh *mesh) {
//     auto vertices = process_vertices(mesh);
//     auto indices = process_indices(mesh);
//     return Mesh { vertices, indices };
// }
//
// std::vector<Vertex> AssimpModelLoader::process_vertices(aiMesh *mesh) {
//     std::vector<Vertex> vertices {};
//     for (int i = 0; i < mesh->mNumVertices; i++) {
//         glm::vec3 pos { to_glm_vec3(mesh->mVertices[i]) };
//         glm::vec3 norm { to_glm_vec3(mesh->mNormals[i]) };
//         glm::vec2 tex_coords = (mesh->mTextureCoords[0])
//             ? to_glm_vec2(*mesh->mTextureCoords[i])
//             : glm::vec2(0.0f, 0.0f);
//         vertices.push_back(Vertex { pos, norm, tex_coords });
//     }
//     return vertices;
// }
//
// std::vector<uint32_t> AssimpModelLoader::process_indices(aiMesh *mesh) {
//     std::vector<uint32_t> indices{};
//     for (int i = 0; i < mesh->mNumFaces; i++) {
//         aiFace face = mesh->mFaces[i];
//         for (int j = 0; j < face.mNumIndices; j++)
//             indices.push_back(face.mIndices[j]);
//     }
//     return indices;
// }
//
// }
