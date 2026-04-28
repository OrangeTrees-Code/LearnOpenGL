#include <cstddef>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <stb_image.h>
#include <GLFW/glfw3.h>
// #include <cmath>
#include "Window.hpp"
#include "Shader.hpp"
#include <filesystem>
#include <iostream>

int main() {
  GLFWwindow * window;
  Window winInstance(800,600);
  if ((window = winInstance.WindowInit(true)) == NULL) {
    glfwTerminate();
    return -1;
  }


  glm::mat4 ortho = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
  glm::mat4 persp = glm::perspective((float)glm::radians(45.0), float(winInstance.Width)/float(winInstance.Height), 0.1f, 100.0f);


  glm::mat4 model;
  model = glm::rotate(model, glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 view;
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection = glm::perspective((float)glm::radians(45.0), float(winInstance.Width)/float(winInstance.Height), 0.1f, 100.0f);
  
}