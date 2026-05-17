#pragma once

#include "Camera.hpp"
#include "Cursor.hpp"
// OpenGL loader
#include <glad/glad.h>
// Windowing
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <memory>
#include <stdexcept>

class Window {
public:
  float deltaTime = 0.0f;
  const int Width = 0;
  const int Height = 0;
  Cursor MainCursor;
  Camera &MainCamera;

public:
  Window(int width, int height, bool isTransparent, Camera &mainCamera,
         float cursorSensitivity = 0.005f, float scrollSensitivity = 1.0f)
      : Width(width), Height(height), window_ptr_(WindowInit(isTransparent)),
        MainCursor(Width / 2, Height / 2, cursorSensitivity, scrollSensitivity),
        MainCamera(mainCamera) {
    if (window_ptr_.get() == NULL) {
      throw std::runtime_error("Window Create Fail!");
    }
  };
  GLFWwindow *Get_Window_Ptr() { return window_ptr_.get(); }

  void processInput(Camera &camera, std::vector<glm::vec3> &lightPos);
  void UpdateDeltaTime();

private:
  struct GLFWwindowDeleter {
    void operator()(GLFWwindow *w) const {
      if (w)
        glfwDestroyWindow(w);
    }
  };
  using WindowPtr = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>;

private:
  float currentFrame = 0.0f;
  float lastFrame = 0.0f;
  WindowPtr window_ptr_;

private:
  GLFWwindow *WindowInit(bool isTransparent);

  void framebuffer_size_callback(GLFWwindow *window, int width, int height);
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
};