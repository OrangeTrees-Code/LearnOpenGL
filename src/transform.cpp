#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

int main() {
  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans(1.0f);
  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
  vec = trans * vec;
  std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;

  glm::mat4 trans2(1.0f);
  trans2 = glm::rotate(trans2, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  trans2 = glm::scale(trans2, glm::vec3(0.5f, 0.5f, 0.5f));
  
}