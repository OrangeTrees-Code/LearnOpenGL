#pragma once
#include <glm/ext/vector_float3.hpp>
class Color {
public:
  static glm::vec3 RGBtoHSV(const glm::vec3 &rgb);

  static glm::vec3 HSVtoRGB(const glm::vec3 &hsv);
};