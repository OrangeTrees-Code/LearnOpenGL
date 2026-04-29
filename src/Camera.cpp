#include "Camera.hpp"
#include "Global.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

glm::mat4 Camera::GetViewMatrix() {
  // return glm::lookAt(Position, Position + Front, Up);
  return Global::Instance().MyLookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
  return glm::perspective(glm::radians(FOV),
                          (float)ViewWidth / (float)ViewHeight, 0.1f, 100.0f);
}

void Camera::UpdateCameraVec() {
  glm::vec3 front;

  const float pitch = glm::radians(Pitch);
  const float yaw = glm::radians(Yaw);
  front.x = std::cos(pitch) * std::cos(yaw);
  front.y = std::sin(pitch);
  front.z = std::cos(pitch) * std::sin(yaw);
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, Global::Instance().Up));
  Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessScroll(float xoffset, float yoffset) {
  if (FOV >= 1.0f && FOV <= 150.0f) {
    FOV -= yoffset;
  }
  if (FOV < 1.0f) {
    FOV = 1.0f;
  }
  if (FOV > 150.0f) {
    FOV = 120.0f;
  }
}

void Camera::ProcessMouseMove(float xoffset, float yoffset,
                              bool constrainPitch) {
  Yaw += xoffset;
  Pitch += yoffset;

  if (constrainPitch == true) {
    if (Pitch > 89.0f) {
      Pitch = 89.0f;
    }

    if (Pitch < -89.0f) {
      Pitch = -89.0f;
    }
  }
  UpdateCameraVec();
}

void Camera::ProcessKeyInput(Direction direction, float deltaTime) {
  float v = MoveSpeed * deltaTime;
  switch (direction) {
  case Direction::Forward:
    Position += Front * v;
    break;
  case Direction::Backward:
    Position -= Front * v;
    break;
  case Direction::Left:
    Position -= Right * v;
    break;
  case Direction::Right:
    Position += Right * v;
    break;
  case Direction::Up:
    Position += Up * v;
    break;
  case Direction::Down:
    Position -= Up * v;
    break;
  }
  // Position.y = 0.0f;
}
