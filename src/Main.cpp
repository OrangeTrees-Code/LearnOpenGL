// OpenGL loader
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
// Windowing
#include "Camera.hpp"
#include "Shader.tpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <print>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
int main() {
  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), 2560, 1600);

    Window window(2560, 1600, false, camera, 0.5f, 2.0f);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
        0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
        0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
        0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
        0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
        0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

    float floorVertices[] = {-10.0, -0.5f, -10.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             10.0,  -0.5f, -10.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                             10.0,  -0.5f, 10.0,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             10.0,  -0.5f, 10.0,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             -10.0, -0.5f, 10.0,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                             -10.0, -0.5f, -10.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    unsigned int VAO, VBO[2];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glad_glBindVertexArray(0);

    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);

    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    std::vector<glm::vec3> objectPos{glm::vec3(0.0f, 10.0f, 0.0f)};

    std::vector<glm::vec3> lightPos{glm::vec3(1.2f, 1.0f, 2.0f)};

    std::filesystem::path ProjRoot(PROJECT_ROOT);
    auto loadTexture = [](const std::filesystem::path &path) {
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      int width, height, channels;
      unsigned char *data =
          stbi_load(path.c_str(), &width, &height, &channels, 0);
      if (data == nullptr) {
        throw std::runtime_error("Failed to load texture: " + path.string());
      }

      GLenum format = GL_RGB;
      if (channels == 1) {
        format = GL_RED;
      } else if (channels == 4) {
        format = GL_RGBA;
      }

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      stbi_image_free(data);

      return texture;
    };
    unsigned int woodTexture = loadTexture(ProjRoot / "textures/wood.png");

    Shader objShader(ProjRoot / "src/vertShader.vert",
                     ProjRoot / "src/objectFrag.frag");

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    objShader.Use();
    objShader.SetUniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    objShader.SetUniform("lightColor", lightColor);
    objShader.SetUniform("ambientStrength", 0.1f);

    objShader.SetUniform("specularStrength", 0.5f);
    objShader.SetUniform("Shininess", 32);
    objShader.SetUniform("diffuseTexture", 0);

    Shader lightShader(ProjRoot / "src/vertShader.vert",
                       ProjRoot / "src/lightFrag.frag");

    lightShader.Use();
    lightShader.SetUniform("lightColor", lightColor);

    glBindVertexArray(VAO);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
      window.processInput(camera);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      objShader.Use();

      objShader.SetUniform("viewPos", camera.Position);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      auto view = camera.GetViewMatrix();
      auto projection = camera.GetProjectionMatrix();

      objShader.SetUniform("view", view);
      objShader.SetUniform("projection", projection);

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        lightPos[0] += 1 * window.deltaTime * camera.Front;
      }

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        lightPos[0] -= 1 * window.deltaTime * camera.Front;
      }
      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_UP) == GLFW_PRESS) {
        lightPos[0].y += window.deltaTime * 1;
      }

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        lightPos[0].y -= window.deltaTime * 1;
      }

      // 也许可以考虑多个光源，而不是硬编码第一个光源
      objShader.SetUniform("lightPos", lightPos[0]);

      for (auto &pos : objectPos) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

        objShader.SetUniform("useTexture", false);
        objShader.SetUniform("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, woodTexture);
      glBindVertexArray(floorVAO);
      objShader.SetUniform("useTexture", true);
      objShader.SetUniform("model", glm::mat4(1.0f));
      glDrawArrays(GL_TRIANGLES, 0, 6);

      lightShader.Use();
      lightShader.SetUniform("view", view);
      lightShader.SetUniform("projection", projection);
      glBindVertexArray(VAO);

      for (auto &pos : lightPos) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightShader.SetUniform("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      window.UpdateDeltaTime();

      glfwSwapBuffers(window.Get_Window_Ptr());
      glfwPollEvents();
    }

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteTextures(1, &woodTexture);

  } catch (std::exception &exce) {
    std::println("{}", exce.what());
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
