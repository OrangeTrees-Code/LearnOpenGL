#include "Mesh.hpp"
#include "Shader.tpp"
#include <cstddef>
#include <stb_image.h>
#include <string>

void Mesh::SetUpMesh() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
               indices_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Position));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) {
  unsigned int diffIndex = 0;
  unsigned int specIndex = 0;

  for (int i = 0; i < textures_.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    std::string name;
    switch (textures_.at(i).Type) {

    case E_TexType::Diffuse:
      name = "texture_diffuse_" + std::to_string(diffIndex++);
      break;
    case E_TexType::Specular:
      name = "texture_specular_" + std::to_string(specIndex++);
      break;
    }

    shader.SetUniform(name, i);
    glBindTexture(GL_TEXTURE_2D, textures_.at(i).ID);
  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

unsigned int Texture::LoadTexture(std::filesystem::path path2Tex, bool isSetSlot, int slotIndex, GLint wrapS, GLint wrapT) {
  unsigned int texture;
  glGenTextures(1, &texture);
  if (isSetSlot == true) {
    glActiveTexture(GL_TEXTURE0 + slotIndex);
  }
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(path2Tex.c_str(), &width, &height, &nrChannels, 0);
  if (data != NULL) {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("Failed to load texture");
  }

  stbi_image_free(data);

  return texture;
}
