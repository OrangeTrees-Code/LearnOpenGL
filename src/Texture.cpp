#include "Texture.hpp"
#include <glad/glad.h>
#include <stb_image.h>
#include <vector>

unsigned int Texture::loadTexture(const std::filesystem::path &path, bool gammaCorrection) {
  unsigned int texture;

  int width, height, channels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data == nullptr) {
    stbi_image_free(data);
    throw std::runtime_error("Failed to load texture: " + path.string());
  }

  glGenTextures(1, &texture);
  GLenum dataFormat = GL_RGB;
  GLenum internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
  if (channels == 1) {
    dataFormat = GL_RED;
    internalFormat = GL_RED;
  } else if (channels == 4) {
    dataFormat = GL_RGBA;
    internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);
  return texture;
};

unsigned int Texture::createWhiteTexture() {
  unsigned int texture;
  std::vector<unsigned char> pixel = {255};
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE,
               pixel.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
};

unsigned int Texture::loadTexture_HDR(const std::filesystem::path &path, bool gammaCorrection) {
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrComponents;
  float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);

  unsigned int hdrTexture;
  if (data == nullptr) {
    stbi_image_free(data);
    throw std::runtime_error("Failed to load texture: " + path.string());
  }

  glGenTextures(1, &hdrTexture);
  glBindTexture(GL_TEXTURE_2D, hdrTexture);

  GLenum dataFormat = GL_RGB;
  GLenum internalFormat = GL_RGB16F;
  if (nrComponents == 1) {
    dataFormat = GL_RED;
    internalFormat = GL_R16F;
  } else if (nrComponents == 4) {
    dataFormat = GL_RGBA;
    internalFormat = GL_RGBA16F;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat,
               GL_FLOAT, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);
  stbi_set_flip_vertically_on_load(false);
  return hdrTexture;
}
