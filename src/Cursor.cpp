#include "Cursor.hpp"

glm::vec2 Cursor::GetOffset(glm::vec2 curr_pos) {
  glm::vec2 offset = curr_pos - Position;
  offset.y *= -1;
  Position = curr_pos;
  return offset;
}