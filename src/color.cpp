#include <glm/ext/vector_float3.hpp>
int main() {
  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
  glm::vec3 objColor(1.0f, 0.5f, 0.31f);

  glm::vec3 result = lightColor * objColor;

  // glm::vec3 lightColor(0.0f, 1.0f, 0.0f);
  // glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
  // glm::vec3 result = lightColor * toyColor; // = (0.0f, 0.5f, 0.0f);

  // glm::vec3 lightColor(0.33f, 0.42f, 0.18f);
  // glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
  // glm::vec3 result = lightColor * toyColor; // = (0.33f, 0.21f, 0.06f);
}