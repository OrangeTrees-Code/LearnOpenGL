#include "Color.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

glm::vec3 Color::HSVtoRGB(const glm::vec3 &hsv) {
  float h = hsv.x;
  float s = hsv.y;
  float v = hsv.z;

  float c = v * s;
  float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
  float m = v - c;

  glm::vec3 rgb(0.0f);

  if (h < 60.0f)
    rgb = glm::vec3(c, x, 0.0f);
  else if (h < 120.0f)
    rgb = glm::vec3(x, c, 0.0f);
  else if (h < 180.0f)
    rgb = glm::vec3(0.0f, c, x);
  else if (h < 240.0f)
    rgb = glm::vec3(0.0f, x, c);
  else if (h < 300.0f)
    rgb = glm::vec3(x, 0.0f, c);
  else
    rgb = glm::vec3(c, 0.0f, x);

  return rgb + glm::vec3(m);
}

glm::vec3 Color::RGBtoHSV(const glm::vec3 &rgb) {
  float r = rgb.x;
  float g = rgb.y;
  float b = rgb.z;

  float maxVal = std::max({r, g, b});
  float minVal = std::min({r, g, b});
  float delta = maxVal - minVal;

  float h = 0.0f;
  float s = 0.0f;
  float v = maxVal;

  if (delta > 0.0f) {
    if (maxVal == r)
      h = 60.0f * std::fmod((g - b) / delta, 6.0f);
    else if (maxVal == g)
      h = 60.0f * (((b - r) / delta) + 2.0f);
    else
      h = 60.0f * (((r - g) / delta) + 4.0f);

    if (h < 0.0f)
      h += 360.0f;
  }

  if (maxVal > 0.0f)
    s = delta / maxVal;

  return glm::vec3(h, s, v);
}