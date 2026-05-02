// OpenGL loader
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
// Windowing
#include "Camera.hpp"
#include "Color.hpp"
#include "Shader.tpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <print>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

unsigned int LoadTexture(std::filesystem::path path2Tex, int slotIndex);

int main() {
  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), 800, 600);

    Window window(800, 600, true, camera, 0.5f, 2.0f);

#pragma region Data

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
        -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
        0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
        1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
        0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
        0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
        -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
        0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
        -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
        0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
        -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
        -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
        1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
        -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
        1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

    std::vector<glm::vec3> objectPos{
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    std::vector<glm::vec3> lightPos{glm::vec3(1.2f, 1.0f, 2.0f)};

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    std::filesystem::path ProjRoot(PROJECT_ROOT);

#pragma endregion

#pragma region Init_VAO_VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
#pragma endregion

#pragma region Init_Texture
    std::vector<unsigned int> textures(3);
    textures[0] = LoadTexture(ProjRoot / "textures/container2.png", 0);
    textures[1] =
        LoadTexture(ProjRoot / "textures/lighting_maps_specular_color.png", 1);
    // textures[2] = LoadTexture(ProjRoot / "textures/matrix.jpg", 2);
#pragma endregion

#pragma region Init_Shader
    Shader objShader(ProjRoot / "src/vertShader.vert",
                     ProjRoot / "src/objectFrag.frag");

    objShader.Use();
    objShader.SetUniform("light.color", lightColor);
    // 也许可以考虑多个光源，而不是硬编码第一个光源
    // objShader.SetUniform("light.vector", glm::vec4(lightPos[0], 1.0f));
    // objShader.SetUniform("light.vector", glm::vec4(-0.2f, -1.0f, -0.3f,
    // 0.0f));

    objShader.SetUniform("light.position", camera.Position);
    objShader.SetUniform("light.direction", camera.Front);
    objShader.SetUniform("light.cutOff", glm::cos(glm::radians(12.5f)));
    objShader.SetUniform("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    // objShader.SetUniform("light.constant", 1.0f);
    // objShader.SetUniform("light.linear", 0.09f);
    // objShader.SetUniform("light.quadratic", 0.032f);
    objShader.SetUniform("light.ambientStrength", glm::vec3(0.2f, 0.2f, 0.2f));
    objShader.SetUniform("light.diffuseStrength", glm::vec3(0.5f, 0.5f, 0.5f));
    objShader.SetUniform("light.specularStrength", glm::vec3(1.0f, 1.0f, 1.0f));
    objShader.SetUniform("material.diffuseTex", 0);
    objShader.SetUniform("material.specularTex", 1);
    // objShader.SetUniform("material.emissionTex", 2);
    objShader.SetUniform("material.shininess", 32.0f);

    Shader lightShader(ProjRoot / "src/vertShader.vert",
                       ProjRoot / "src/lightFrag.frag");

    lightShader.Use();
    lightShader.SetUniform("lightColor", lightColor);
#pragma endregion

#pragma region SetUp_Rendering
    glBindVertexArray(VAO);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float time;
#pragma endregion

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
#pragma region Before_Rendering
      window.processInput(camera);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma endregion

#pragma region Rendering
      objShader.Use();

      objShader.SetUniform("light.position", camera.Position);
      objShader.SetUniform("light.direction", camera.Front);

      objShader.SetUniform("viewPos", camera.Position);

      auto view = camera.GetViewMatrix();
      auto projection = camera.GetProjectionMatrix();

      objShader.SetUniform("view", view);
      objShader.SetUniform("projection", projection);

      for (int i = 0; i < objectPos.size(); i++) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), objectPos[i]);
        model = glm::rotate(model, glm::radians(20.0f * i),
                            glm::vec3(1.0f, 1.0f, 1.0f));

        objShader.SetUniform("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      lightShader.Use();

      lightShader.SetUniform("view", view);
      lightShader.SetUniform("projection", projection);

      for (auto &pos : lightPos) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightShader.SetUniform("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
#pragma endregion

#pragma region After_Rendering
      window.UpdateDeltaTime();

      glfwSwapBuffers(window.Get_Window_Ptr());
      glfwPollEvents();
#pragma endregion
    }

#pragma region FollowUp_Rendering
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
#pragma endregion

  } catch (std::exception &exce) {
    std::println("{}", exce.what());
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

unsigned int LoadTexture(std::filesystem::path path2Tex, int slotIndex) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0 + slotIndex);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(path2Tex.c_str(), &width, &height, &nrChannels, 0);
  if (data != NULL) {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("Failed to load texture");
  }

  stbi_image_free(data);

  return texture;
}
