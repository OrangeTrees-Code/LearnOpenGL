#include "CharacterSet.hpp"
#include "Shader.tpp"
#include <freetype/freetype.h>
#include <freetype2/ft2build.h>
#include <ranges>
#include <stdexcept>
#include FT_FREETYPE_H

void CharacterSet::Init() {
  FT_Face face;
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
  }

  if (FT_New_Face(ft, "/usr/share/fonts/TTF/JetBrainsMonoNerdFontMono-Thin.ttf", 0, &face)) {
    FT_Done_FreeType(ft);
    throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  //   if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
  //     throw "ERROR::FREETYTPE: Failed to load Glyph";
  //   }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (GLubyte c : std::ranges::views::iota(0u, 128u)) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      continue;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    struct Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<GLuint>(face->glyph->advance.x)};

    Characters_[c] = character;
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  Generate_VAO_VBO_();
}

void CharacterSet::Generate_VAO_VBO_() {
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void CharacterSet::RenderText(Shader &s, const std::string &text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color) {
  s.Use();
  s.SetUniform("color", color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO_);

  for (const char &c : text) {
    Character ch = Characters_[c];

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    GLfloat vertices[6][4] = {
        {xpos, ypos + h, 0.0, 0.0},
        {xpos, ypos, 0.0, 1.0},
        {xpos + w, ypos, 1.0, 1.0},

        {xpos, ypos + h, 0.0, 0.0},
        {xpos + w, ypos, 1.0, 1.0},
        {xpos + w, ypos + h, 1.0, 0.0}};
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += (ch.Advance >> 6) * scale;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
