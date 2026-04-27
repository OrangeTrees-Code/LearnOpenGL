#ifndef SHADER_H
#define SHADER_H
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

private:
  unsigned int ID_;
  int success;
  char infoLog[512];

public:
  Shader(std::filesystem::path vertexShaderSrc,
         std::filesystem::path fragmentShaderSrc);
  void Use() const;

  unsigned int Get_ID() const;

  void SetInt(const std::string &name, int value) const;

  void SetBool(const std::string &name, bool value) const;

  void SetFloat(const std::string &name, float value) const;

  void SetVec3(const std::string &name, const glm::vec3 &vector) const;

private:
  unsigned int InitVertexShader(const char *vertexShaderSource);

  unsigned int InitFragmentShader(const char *fragmentShaderSource);

  unsigned int InitShaderProgram(unsigned int vertexShader,
                                 unsigned int fragmentShader);
};

#endif