// OpenGL loader
#include <cstddef>
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
  const int SCR_WIDTH = 2560;
  const int SCR_HEIGHT = 1600;

  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);

    Window window(SCR_WIDTH, SCR_HEIGHT, false, camera, 0.5f, 2.0f);

    float vertices[] = {
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

    float floorVertices[] = {-3.0f, -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             3.0f,  -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                             3.0f,  -0.5f, 3.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             3.0f,  -0.5f, 3.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             -3.0f, -0.5f, 3.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                             -3.0f, -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

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

    const int shadowWidth = 2048;
    const int shadowHeight = 2048;
    const float nearPlane = 0.1f;
    const float farPlane = 25.0f;

    unsigned int shadowMap;
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
    for (unsigned int i = 0; i < 6; ++i) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                   shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                   NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<glm::vec3> objectPos{glm::vec3(0.0f, 0.5f, 0.0f)};

    std::vector<glm::vec3> lightPos{glm::vec3(1.f, 1.f, 1.f)};

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
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      float borderColor[] = {1.0, 1.0, 1.0, 1.0};
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

      int width, height, channels;
      unsigned char *data =
          stbi_load(path.c_str(), &width, &height, &channels, 0);
      if (data == nullptr) {
        throw std::runtime_error("Failed to load texture: " + path.string());
      }

      GLenum format = GL_RGB, sFormet = GL_SRGB;
      if (channels == 1) {
        format = GL_RED;
      } else if (channels == 4) {
        format = GL_RGBA;
        sFormet = GL_SRGB_ALPHA;
      }

      glTexImage2D(GL_TEXTURE_2D, 0, sFormet, width, height, 0, format,
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
    objShader.SetUniform("ambientStrength", 0.001f);

    objShader.SetUniform("specularStrength", 0.5f);
    objShader.SetUniform("Shininess", 32);
    objShader.SetUniform("diffuseTexture", 0);
    objShader.SetUniform("farPlane", farPlane);

    Shader lightShader(ProjRoot / "src/vertShader.vert",
                       ProjRoot / "src/lightFrag.frag");

    lightShader.Use();
    lightShader.SetUniform("lightColor", lightColor);

    Shader depthShader(ProjRoot / "src/depth.vert",
                       ProjRoot / "src/depth.frag");

    glBindVertexArray(VAO);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_MULTISAMPLE);

    objShader.Use();
    objShader.SetUniform("shadowMap", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    objShader.SetUniform("useTexture", true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
      window.processInput(camera);

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        lightPos[0] += 1 * window.deltaTime * camera.Front;
      }

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        lightPos[0] -= 1 * window.deltaTime * camera.Front;
      }
      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_UP) == GLFW_PRESS) {
        lightPos[0].y += window.deltaTime * 0.5;
      }

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        lightPos[0].y -= window.deltaTime * 0.5;
      }

      glViewport(0, 0, shadowWidth, shadowHeight);
      glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

      depthShader.Use();
      depthShader.SetUniform("lightPos", lightPos[0]);
      depthShader.SetUniform("farPlane", farPlane);

      auto shadowProjection = glm::perspective(
          glm::radians(90.0f),
          static_cast<float>(shadowWidth) / static_cast<float>(shadowHeight),
          nearPlane, farPlane);
      glm::mat4 shadowTransforms[] = {
          shadowProjection *
              glm::lookAt(lightPos[0],
                          lightPos[0] + glm::vec3(1.0f, 0.0f, 0.0f),
                          glm::vec3(0.0f, -1.0f, 0.0f)),
          shadowProjection *
              glm::lookAt(lightPos[0],
                          lightPos[0] + glm::vec3(-1.0f, 0.0f, 0.0f),
                          glm::vec3(0.0f, -1.0f, 0.0f)),
          shadowProjection *
              glm::lookAt(lightPos[0],
                          lightPos[0] + glm::vec3(0.0f, 1.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f)),
          shadowProjection *
              glm::lookAt(lightPos[0],
                          lightPos[0] + glm::vec3(0.0f, -1.0f, 0.0f),
                          glm::vec3(0.0f, 0.0f, -1.0f)),
          shadowProjection *
              glm::lookAt(lightPos[0],
                          lightPos[0] + glm::vec3(0.0f, 0.0f, 1.0f),
                          glm::vec3(0.0f, -1.0f, 0.0f)),
          shadowProjection *
              glm::lookAt(lightPos[0],
                          lightPos[0] + glm::vec3(0.0f, 0.0f, -1.0f),
                          glm::vec3(0.0f, -1.0f, 0.0f)),
      };

      for (unsigned int face = 0; face < 6; ++face) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, shadowMap,
                               0);
        glClear(GL_DEPTH_BUFFER_BIT);

        depthShader.SetUniform("lightSpaceMatrix", shadowTransforms[face]);

        for (auto &pos : objectPos) {
          glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

          depthShader.SetUniform("model", model);

          glBindVertexArray(VAO);
          glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(floorVAO);

        depthShader.SetUniform("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
      }

      // glDisable(GL_CULL_FACE);
      ////////////////////////////////////////////////////////////////////////////////////////

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      int fbWidth, fbHeight;
      glfwGetFramebufferSize(window.Get_Window_Ptr(), &fbWidth, &fbHeight);
      glViewport(0, 0, fbWidth, fbHeight);
      camera.ViewWidth = fbWidth;
      camera.ViewHeight = fbHeight;

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      objShader.Use();

      objShader.SetUniform("viewPos", camera.Position);

      auto view = camera.GetViewMatrix();
      auto projection = camera.GetProjectionMatrix();

      objShader.SetUniform("view", view);
      objShader.SetUniform("projection", projection);

      // 也许可以考虑多个光源，而不是硬编码第一个光源
      objShader.SetUniform("lightPos", lightPos[0]);

      for (auto &pos : objectPos) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

        objShader.SetUniform("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glBindVertexArray(floorVAO);

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
    glDeleteBuffers(1, &VBO);
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
