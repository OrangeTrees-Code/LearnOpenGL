#include "Shader.tpp"
// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(std::filesystem::path vertexShaderSrc,
               std::filesystem::path fragmentShaderSrc) {
  std::ifstream vertSrc;
  std::ifstream fragSrc;

  std::stringstream buffer_vertSrc;
  std::stringstream buffer_fragSrc;

  vertSrc.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  fragSrc.exceptions(std::ifstream::badbit | std::ifstream::failbit);

  try {
    vertSrc.open(vertexShaderSrc);

    buffer_vertSrc << vertSrc.rdbuf();
    vertSrc.close();

    fragSrc.open(fragmentShaderSrc);

    buffer_fragSrc << fragSrc.rdbuf();
    fragSrc.close();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  unsigned int vertexShader =
      this->InitVertexShader(buffer_vertSrc.str().c_str());

  unsigned int fragmentShader =
      this->InitFragmentShader(buffer_fragSrc.str().c_str());

  this->ID_ = this->InitShaderProgram(vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

unsigned int Shader::InitVertexShader(const char *vertexShaderSource) {
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_);

  if (!success_) {
    glGetShaderInfoLog(vertexShader, sizeof(infoLog_), NULL, infoLog_);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog_ << std::endl;
  }

  return vertexShader;
}

unsigned int Shader::InitFragmentShader(const char *fragmentShaderSource) {
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_);

  if (!success_) {
    glGetShaderInfoLog(fragmentShader, sizeof(infoLog_), NULL, infoLog_);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog_ << std::endl;
  }

  return fragmentShader;
}

unsigned int Shader::InitShaderProgram(unsigned int vertexShader,
                                       unsigned int fragmentShader) {
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_);
  if (!success_) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog_);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog_ << std::endl;
  }

  return shaderProgram;
}

void Shader::Use() const { glUseProgram(Get_ID()); }

unsigned int Shader::Get_ID() const { return ID_; }

int Shader::GetUniformLocation(const std::string &name) const {
  if (uniformCache_.contains(name)) {
    return uniformCache_[name];
  }

  int location = glGetUniformLocation(this->Get_ID(), name.c_str());
  uniformCache_[name] = location;
  return location;
}
