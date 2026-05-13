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
#include <string>
#include <vector>
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

    float floorVertices[] = {-3.0f, -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             3.0f,  -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                             3.0f,  -0.5f, 3.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             3.0f,  -0.5f, 3.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             -3.0f, -0.5f, 3.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                             -3.0f, -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    float screenQuadVertices[] = {
        -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
        1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f,
    };

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

    unsigned int screenQuadVAO, screenQuadVBO;
    glGenVertexArrays(1, &screenQuadVAO);
    glGenBuffers(1, &screenQuadVBO);

    glBindVertexArray(screenQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices),
                 screenQuadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    std::vector<glm::vec3> objectPos{glm::vec3(-1.2f, 0.0f, -0.8f),
                                     glm::vec3(1.2f, 0.0f, -1.1f),
                                     glm::vec3(0.0f, 0.7f, 0.4f)};

    std::vector<glm::vec3> lightPos{
        glm::vec3(0.0f, 1.4f, 1.3f), glm::vec3(-2.0f, 0.8f, -1.5f),
        glm::vec3(2.0f, 0.8f, -1.2f), glm::vec3(0.0f, 2.4f, -2.0f)};
    std::vector<glm::vec3> lightColors{
        glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(8.0f, 1.3f, 0.7f),
        glm::vec3(0.7f, 2.0f, 8.0f), glm::vec3(1.0f, 8.0f, 2.0f)};

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

    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window.GetFrameBufferWidth(),
                 window.GetFrameBufferHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int depthRBO;
    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                          window.GetFrameBufferWidth(),
                          window.GetFrameBufferHeight());

    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, depthRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      throw std::runtime_error(
          "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader objShader(ProjRoot / "src/vertShader.vert",
                     ProjRoot / "src/hdr_scene.frag");
    Shader hdrScreenShader(ProjRoot / "src/hdr_screen.vert",
                           ProjRoot / "src/hdr_screen.frag");

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    objShader.Use();
    objShader.SetUniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    objShader.SetUniform("ambientStrength", 0.03f);
    objShader.SetUniform("specularStrength", 0.5f);
    objShader.SetUniform("Shininess", 32);
    objShader.SetUniform("diffuseTexture", 0);

    hdrScreenShader.Use();
    hdrScreenShader.SetUniform("hdrBuffer", 0);
    hdrScreenShader.SetUniform("exposure", 1.0f);

    Shader lightShader(ProjRoot / "src/vertShader.vert",
                       ProjRoot / "src/lightFrag.frag");

    lightShader.Use();
    lightShader.SetUniform("lightColor", lightColor);

    glBindVertexArray(VAO);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_MULTISAMPLE);

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
      window.processInput(camera);

      glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.02f, 0.02f, 0.025f, 1.0f);
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
        lightPos[0].y += window.deltaTime * 0.5;
      }

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        lightPos[0].y -= window.deltaTime * 0.5;
      }

      objShader.SetUniform("lightCount", static_cast<int>(lightPos.size()));
      for (std::size_t i = 0; i < lightPos.size(); ++i) {
        objShader.SetUniform("lights[" + std::to_string(i) + "].Position",
                             lightPos[i]);
        objShader.SetUniform("lights[" + std::to_string(i) + "].Color",
                             lightColors[i]);
      }

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

      for (std::size_t i = 0; i < lightPos.size(); ++i) {
        auto &pos = lightPos[i];
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightShader.SetUniform("lightColor", lightColors[i]);
        lightShader.SetUniform("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      hdrScreenShader.Use();
      hdrScreenShader.SetUniform("exposure", 1.0f);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, colorBuffer);
      glBindVertexArray(screenQuadVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      window.UpdateDeltaTime();

      glfwSwapBuffers(window.Get_Window_Ptr());
      glfwPollEvents();
    }

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteVertexArrays(1, &screenQuadVAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &screenQuadVBO);
    glDeleteTextures(1, &woodTexture);
    glDeleteTextures(1, &colorBuffer);
    glDeleteRenderbuffers(1, &depthRBO);
    glDeleteFramebuffers(1, &hdrFBO);

  } catch (std::exception &exce) {
    std::println("{}", exce.what());
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
