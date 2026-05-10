#include "Model.hpp"
#include "Mesh.hpp"
#include "Shader.tpp"
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <cstddef>
#include <filesystem>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <stb_image.h>
#include <stdexcept>

std::unordered_map<std::filesystem::path, Texture> Model::textureCache_;

void Model::Draw(Shader &shader) {
  for (auto &mesh : meshes_) {
    mesh.Draw(shader);
  }
}

void Model::LoadModel(const std::filesystem::path &modelPath) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(modelPath.c_str(), aiProcess_Triangulate);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    throw std::runtime_error("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
  }

  directory_ = modelPath.parent_path();

  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.emplace_back(ProcessMesh(mesh, scene));
  }
  for (size_t i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

auto aiVector3D2glmvec3 = [](const aiVector3D &aivec) -> glm::vec3 { return glm::vec3(aivec.x, aivec.y, aivec.z); };

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.Position = aiVector3D2glmvec3(mesh->mVertices[i]);
    vertex.Normal = aiVector3D2glmvec3(mesh->mNormals[i]);
    if (mesh->mTextureCoords[0]) {
      auto &texture_0 = mesh->mTextureCoords[0];
      vertex.TexCoords = glm::vec2(texture_0[i].x, texture_0[i].y);
    } else {
      vertex.TexCoords = glm::vec2(0.0f);
    }

    vertices.emplace_back(vertex);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    auto &face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(end(textures), begin(diffuseMaps), end(diffuseMaps));
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(begin(textures), begin(specularMaps), end(specularMaps));
  }

  return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type) {
  std::vector<Texture> textures;
  for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    auto path = std::filesystem::path(str.C_Str());

    if (textureCache_.contains(path)) {
      textures.push_back(textureCache_[path]);
      continue;
    }

    Texture texture;
    texture.ID = LoadTexture(std::filesystem::path(str.C_Str()), this->directory_);
    switch (type) {
    case aiTextureType_DIFFUSE:
      texture.Type = E_TexType::Diffuse;
      break;
    case aiTextureType_SPECULAR:
      texture.Type = E_TexType::Specular;
      break;
    default:
      texture.Type = E_TexType::Diffuse;
      break;
    }
    texture.Path = str.C_Str();
    textures.push_back(texture);

    textureCache_[path] = texture;
  }
  return textures;
}

unsigned int Model::LoadTexture(const std::filesystem::path &relPath, const std::filesystem::path &directory) {
  std::filesystem::path absPath = directory / relPath;
  unsigned int textureId;
  glGenTextures(1, &textureId);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(absPath.c_str(), &width, &height, &nrComponents, 0);

  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    stbi_image_free(data);
    throw std::runtime_error("Texture failed to load at path : " + absPath.string());
  }

  return textureId;
}

template <class>
inline constexpr bool always_false_v = false;

template <typename T>
void Model::SetInstanceProp(int slot, unsigned int instanceVBO) const {
  if constexpr (std::same_as<T, glm::mat4>) {
    for (unsigned int i = 0; i < meshes_.size(); ++i) {
      auto vao = meshes_[i].vao_;
      size_t vec4Size = sizeof(glm::vec4);
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

      for (int j = 0; j < 4; j++) {
        glEnableVertexAttribArray(slot + j);
        glVertexAttribPointer(slot + j, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(j * vec4Size));
        glVertexAttribDivisor(slot + j, 1);
      }

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
  } else {
    static_assert(always_false_v<T>, "Unsupported uniform type!");
  }
}

template void Model::SetInstanceProp<glm::mat4>(int slot, unsigned int instanceVBO) const;

void Model::DrawBatch(Shader &shader, int amount) {
  for (auto &mesh : meshes_) {
    mesh.DrawBatch(shader, amount);
  }
}
