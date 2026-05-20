#include "Shader.tpp"
#include <freetype/freetype.h>
#include <glad/glad.h>
#include <glm/ext/vector_int2.hpp>
#include <unordered_map>

struct Character {
  GLuint TextureID;
  glm::ivec2 Size;
  glm::ivec2 Bearing;
  GLuint Advance;
};

class CharacterSet {
public:
  CharacterSet() { this->Init(); };
  void Init();
  void RenderText(Shader &s, const std::string &text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color);

private:
  void Generate_VAO_VBO_();

private:
  GLuint VAO_;
  GLuint VBO_;
  std::unordered_map<GLchar, Character> Characters_;
};