#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

class Window {
public:
  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
  static void processInput(GLFWwindow *window);
  static GLFWwindow *WindowInit(bool isTransparent);
};