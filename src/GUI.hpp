#pragma once

#include <GLFW/glfw3.h>
class GUI {
public:
  static GUI &Instance() {
    static GUI instance;
    return instance;
  }

  void Init(GLFWwindow *window);

private:
  GUI() = default;
  ~GUI() = default;
};