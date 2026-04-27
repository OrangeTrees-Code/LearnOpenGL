#include "Shader.hpp"
#include <glad/glad.h>
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>

int main() {
  auto window = Window::WindowInit(true);

  if (window == NULL) {
    glfwTerminate();
    return -1;
  }

  // float vertices[] = {0.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
  // 0.0f};

  float vertices[] = {0.0f,
                      0.5f,
                      0.0f,
                      -std::sqrt(3.0f) / 4.0f,
                      -1.0f / 4.0f,
                      0.0f,
                      std::sqrt(3.0f) / 4.0f,
                      -1.0f / 4.0f,
                      0.0f};

  float colors[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  unsigned int VAO, VBO[2];
  glGenVertexArrays(1, &VAO);
  glGenBuffers(2, VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  std::filesystem::path shaderPath =
      std::filesystem::path(PROJECT_ROOT) / "src";

  Shader shader(shaderPath / "vertShader2.vert",
                shaderPath / "fragShader2.frag");

  glBindVertexArray(VAO);

  while (!glfwWindowShouldClose(window)) {
    Window::processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Use();

    float timeValue = glfwGetTime();
    float x = (std::sin(timeValue) / 2.0f);
    float y = (std::cos(timeValue) / 2.0f);
    shader.SetVec3("offset", glm::vec3(x, y, 0));
    shader.SetFloat("time", timeValue);
    // int vertexColorLocation = glGetUniformLocation(shaderProgram,
    // "ourColor"); glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f,
    // 0.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glBindVertexArray(0);
  glfwTerminate();
  return 0;
}
