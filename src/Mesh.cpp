#include "Mesh.hpp"
#include "Shader.tpp"
#include <cstddef>
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
