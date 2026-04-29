
#define STB_IMAGE_IMPLEMENTATION
// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <stb_image.h>
// #include <cmath>
#include <glm/common.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

int main() {
  glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);

  glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraDirection =
      glm::normalize(glm::vec3(cameraTarget - cameraPos));

  glm::vec3 up(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

  glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

  glm::mat4 view(1.0f);
  view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
}