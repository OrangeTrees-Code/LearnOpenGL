#pragma once

#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <filesystem>
#include <unordered_map>
#include <vector>
class Model {
public:
  Model(const std::filesystem::path &modelPath) { LoadModel(modelPath); }

  void Draw(Shader &shader);

private:
  static std::unordered_map<std::filesystem::path, Texture> textureCache_;
  std::vector<Mesh> meshes_;
  std::filesystem::path directory_;
  void LoadModel(const std::filesystem::path &modelPath);
  void ProcessNode(aiNode *node, const aiScene *scene);
  Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type);
  unsigned int LoadTexture(const std::filesystem::path &relativePath, const std::filesystem::path &directory);
};