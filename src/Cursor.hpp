#pragma once

#include <glm/ext/vector_float2.hpp>
class Cursor {
public:
  glm::vec2 Position;
  float MoveSensitivity;
  float ScrollSensitivity;

  glm::vec2 GetOffset(glm::vec2 curr_pos);

  Cursor(int X, int Y, float moveSensitivity, float scrollSensitivity)
      : Position(X, Y), MoveSensitivity(moveSensitivity),
        ScrollSensitivity(scrollSensitivity) {};
};