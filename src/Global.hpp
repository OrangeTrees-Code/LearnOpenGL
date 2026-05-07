#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#define STB_IMAGE_IMPLEMENTATION
class Global {

public:
  static Global &Instance() {
    static Global instance;
    return instance;
  }

  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
  Global(const Global &other) = delete;
  Global &operator=(const Global &other) = delete;

private:
  Global() = default;
  ~Global() = default;
};