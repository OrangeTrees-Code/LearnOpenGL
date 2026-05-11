#include "Window.hpp"
#include "Camera.hpp"
#include "Cursor.hpp"
// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

void Window::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

void Window::processInput(Camera &camera) {
  if (glfwGetKey(this->Get_Window_Ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(this->Get_Window_Ptr(), true);
  }
  if (glfwGetKey(this->Get_Window_Ptr(), GLFW_KEY_W) == GLFW_PRESS) {
    MainCamera.ProcessKeyInput(Direction::Forward, deltaTime);
  }
  if (glfwGetKey(this->Get_Window_Ptr(), GLFW_KEY_S) == GLFW_PRESS) {
    MainCamera.ProcessKeyInput(Direction::Backward, deltaTime);
  }
  if (glfwGetKey(this->Get_Window_Ptr(), GLFW_KEY_D) == GLFW_PRESS) {
    MainCamera.ProcessKeyInput(Direction::Right, deltaTime);
  }
  if (glfwGetKey(this->Get_Window_Ptr(), GLFW_KEY_A) == GLFW_PRESS) {
    MainCamera.ProcessKeyInput(Direction::Left, deltaTime);
  }
  if (glfwGetKey(this->Get_Window_Ptr(), GLFW_KEY_SPACE) == GLFW_PRESS) {
    MainCamera.ProcessKeyInput(Direction::Up, deltaTime);
  }
  if (glfwGetKey(this->Get_Window_Ptr(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    MainCamera.ProcessKeyInput(Direction::Down, deltaTime);
  }
}

GLFWwindow *Window::WindowInit(bool isTransparent) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GL_MULTISAMPLE, 64);
  if (isTransparent == true) {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
  }

  GLFWwindow *window = glfwCreateWindow(Width, Height, "Suki", NULL, NULL);
  glfwWindowHintString(GLFW_WAYLAND_APP_ID, "Suki");
  if (window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    return NULL;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return NULL;
  }

  // glViewport(0, 0, Width, Height);
  int fbWidth, fbHeight;
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);
  glfwSetWindowUserPointer(window, this);

  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *window, int width, int height) {
        auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
        self->framebuffer_size_callback(window, width, height);
      });
  glfwSetCursorPosCallback(
      window, [](GLFWwindow *window, double xpos, double ypos) {
        auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
        self->mouse_callback(window, xpos, ypos);
      });
  glfwSetScrollCallback(
      window, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
        self->scroll_callback(window, xoffset, yoffset);
      });
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void Window::UpdateDeltaTime() {
  currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void Window::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  static bool firstMouse = false;
  if (firstMouse == false) {
    MainCursor.Position.x = xpos;
    MainCursor.Position.y = ypos;
    firstMouse = true;
  }
  glm::vec2 offset = MainCursor.GetOffset({xpos, ypos});

  offset *= this->MainCursor.MoveSensitivity;

  MainCamera.ProcessMouseMove(offset.x, offset.y, true);
}

void Window::scroll_callback(GLFWwindow *window, double xoffset,
                             double yoffset) {
  xoffset *= MainCursor.ScrollSensitivity;
  yoffset *= MainCursor.ScrollSensitivity;
  MainCamera.ProcessScroll(xoffset, yoffset);
}
