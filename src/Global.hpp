#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
class Global {

public:
  static Global &Instance() {
    static Global instance;
    return instance;
  }

  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
  Global(const Global &other) = delete;
  Global &operator=(const Global &other) = delete;

  // glm::mat4 MyLookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
  //   // 1. forward（注意方向）
  //   glm::vec3 forward = glm::normalize(target - pos);

  //   // 2. right
  //   glm::vec3 right = glm::normalize(glm::cross(up, forward));

  //   // 3. 真正的 up（正交化）
  //   glm::vec3 newUp = glm::cross(forward, right);

  //   // 4. 旋转矩阵
  //   glm::mat4 rotation(glm::vec4(right, 0.0f), glm::vec4(newUp, 0.0f),
  //                      glm::vec4(forward, 0.0f), glm::vec4(0, 0, 0, 1));

  //   // 5. 平移矩阵
  //   glm::mat4 translation(1.0f);
  //   translation[3][0] = -pos.x;
  //   translation[3][1] = -pos.y;
  //   translation[3][2] = -pos.z;

  //   return rotation * translation;
  // }

private:
  Global() = default;
  ~Global() = default;
};