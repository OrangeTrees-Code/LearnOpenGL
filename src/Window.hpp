#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

class Window {
public:
  Window(int width,int height):Width(width),Height(height){};
  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
  static void processInput(GLFWwindow *window);
  GLFWwindow *WindowInit(bool isTransparent);
  const int Width = 0;
  const int Height = 0;
};