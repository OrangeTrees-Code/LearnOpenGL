#include "CharacterSet.hpp"
#include "Shader.tpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <print>

using Path = std::filesystem::path;

int main() {
  try {
    auto window = Window::WindowInit(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Path ProjRoot(PROJECT_ROOT);
    Shader text_Shader(ProjRoot / "src/shaders/font.vert", ProjRoot / "src/shaders/font.frag");

    CharacterSet cs;
    glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0);
    text_Shader.Use();
    text_Shader.SetUniform("projection", projection);
    text_Shader.SetUniform("text", 0);
    while (!glfwWindowShouldClose(window)) {
      Window::processInput(window);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      cs.RenderText(text_Shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
      cs.RenderText(text_Shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  } catch (std::exception &excep) {
    std::println("{}", excep.what());
  }

  glfwTerminate();
  return 0;
}
