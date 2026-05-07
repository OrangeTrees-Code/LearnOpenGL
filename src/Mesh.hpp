#pragma once

#include "Shader.tpp"
#include <filesystem>
#include <glm/ext/vector_float3.hpp>
#include <vector>
enum class E_TexType {
  Diffuse,
  Specular,
};

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int ID;
  E_TexType Type;
  std::filesystem::path Path;
  static unsigned int LoadTexture(std::filesystem::path path2Tex, bool isSetSlot = false, int slotIndex = 0, GLint warpS = GL_REPEAT, GLint warpT = GL_REPEAT);
  static unsigned int LoadCubeMap(std::vector<std::filesystem::path> &faces);
};

class Mesh {
public:
  Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned int> &&indices,
       std::vector<Texture> &&textures)
      : vertices_(std::move(vertices)), indices_(std::move(indices)),
        textures_(std::move(textures)) {

    SetUpMesh();
  }

  void Draw(Shader &shader);

private:
  void SetUpMesh();
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;
  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ebo_;
};