// OpenGL loader
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
// Windowing
#include "Camera.hpp"
#include "Model.hpp"
#include "SSAO_Engine.hpp"
#include "Shader.tpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), 1920, 1080);

    Window window(1920, 1080, false, camera, 0.5f, 2.0f);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f,
        0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f,
        0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f,
        -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f,
        -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f,
        1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f,
        1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f,
        0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f,
        0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f,
        0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f,
        0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    float floorVertices[] = {-3.0f, -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             3.0f, -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                             3.0f, -0.5f, 3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             3.0f, -0.5f, 3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                             -3.0f, -0.5f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                             -3.0f, -0.5f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    SSAO_Engine ssao_eng(64);
    auto noiseMatrix = ssao_eng.GetNoiseMatrix();
    auto sampleKernel = ssao_eng.GetSampleKernel();

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

    unsigned int G_buffer;
    glGenFramebuffers(1, &G_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, G_buffer);
    unsigned int gPositionDepth, gNormal, gColorSpec;

    glGenTextures(1, &gPositionDepth);
    glBindTexture(GL_TEXTURE_2D, gPositionDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.GetFrameBufferWidth(),
                 window.GetFrameBufferHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           gPositionDepth, 0);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window.GetFrameBufferWidth(),
                 window.GetFrameBufferHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           gNormal, 0);

    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window.GetFrameBufferWidth(),
                 window.GetFrameBufferHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           gColorSpec, 0);

    GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2};

    glDrawBuffers(3, attachments);

    unsigned int depthRBO;
    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                          window.GetFrameBufferWidth(),
                          window.GetFrameBufferHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      throw std::runtime_error(
          "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float quadVertices[] = {
        // positions   // texcoords
        -1.0f,
        1.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        -1.0f,
        1.0f,
        0.0f,
        -1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glBindVertexArray(0);

    GLuint noiseTexture;
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, noiseMatrix->data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint ssaoFBO;
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    GLuint ssaoBuffer;

    glGenTextures(1, &ssaoBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, window.GetFrameBufferWidth(), window.GetFrameBufferHeight(), 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint ssaoBlurFBO, ssaoBlurBuffer;
    glGenFramebuffers(1, &ssaoBlurFBO);
    glGenTextures(1, &ssaoBlurBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glBindTexture(GL_TEXTURE_2D, ssaoBlurBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, window.GetFrameBufferWidth(), window.GetFrameBufferHeight(), 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurBuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<glm::vec3> objectPos{
        glm::vec3(-4.0f, 0.0f, -4.0f),
        glm::vec3(4.0f, 0.0f, -4.0f),
        glm::vec3(-4.0f, 0.0f, 4.0f),
        glm::vec3(4.0f, 0.0f, 4.0f),
    };

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    lightPositions.reserve(32);
    lightColors.reserve(32);

    for (int z = 0; z < 4; ++z) {
      for (int x = 0; x < 8; ++x) {
        lightPositions.emplace_back(-7.0f + x * 2.0f, 1.0f + (x % 3) * 0.6f,
                                    -6.0f + z * 4.0f);
        lightColors.emplace_back(0.5f + (x % 3) * 0.25f, 0.5f + (z % 2) * 0.35f,
                                 0.7f + ((x + z) % 2) * 0.3f);
      }
    }

    std::filesystem::path ProjRoot(PROJECT_ROOT);

    Model nanosuit(ProjRoot / "res/nanosuit_reflection/nanosuit.obj");

    Shader gBufferShader(ProjRoot / "src/vertShader.vert",
                         ProjRoot / "src/g_buffer.frag");
    Shader quadShader(ProjRoot / "src/screenQuad.vert",
                      ProjRoot / "src/quadDisplay.frag");
    Shader lightingPassShader(ProjRoot / "src/lighting_pass.vert",
                              ProjRoot / "src/lighting_pass.frag");
    Shader lightShader(ProjRoot / "src/vertShader.vert",
                       ProjRoot / "src/lightFrag.frag");
    Shader ssaoShader(ProjRoot / "src/ssao.vert", ProjRoot / "src/ssao.frag");

    Shader ssaoBlurShader(ProjRoot / "src/ssao.vert", ProjRoot / "src/ssao_blur.frag");

    ssaoBlurShader.Use();
    ssaoBlurShader.SetUniform("ssaoInput", 0);

    ssaoShader.Use();
    ssaoShader.SetUniform("gPositionDepth", 0);
    ssaoShader.SetUniform("gNormal", 1);
    ssaoShader.SetUniform("texNoise", 2);
    for (auto i : std::ranges::views::iota(0, 64)) {
      ssaoShader.SetUniform("samples[" + std::to_string(i) + "]", sampleKernel->at(i));
    }

    gBufferShader.Use();
    gBufferShader.SetUniform("texture_diffuse_0", 0);
    gBufferShader.SetUniform("texture_specular_0", 1);

    quadShader.Use();
    quadShader.SetUniform("gPositionDepth", 0);
    quadShader.SetUniform("gNormal", 1);
    quadShader.SetUniform("gColorSpec", 2);
    quadShader.SetUniform("texture_ssao", 3);
    quadShader.SetUniform("debugMode", 0);

    lightingPassShader.Use();
    lightingPassShader.SetUniform("gPositionDepth", 0);
    lightingPassShader.SetUniform("gNormal", 1);
    lightingPassShader.SetUniform("gColorSpec", 2);
    lightingPassShader.SetUniform("texture_ssao", 3);
    for (std::size_t i = 0; i < lightPositions.size(); ++i) {
      const std::string lightName = "lights[" + std::to_string(i) + "]";
      lightingPassShader.SetUniform(lightName + ".color", lightColors[i]);
    }

    glBindVertexArray(VAO);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_MULTISAMPLE);

    // GLint encoding;
    // glGetFramebufferAttachmentParameteriv(
    //     GL_FRAMEBUFFER, GL_BACK_LEFT,
    //     GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &encoding);

    // if (encoding == GL_SRGB) {
    //   std::println("SRGB");
    // } else if (encoding == GL_LINEAR) {
    //   std::println("LINEAR RGB");
    // }

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
      window.processInput(camera);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      auto view = camera.GetViewMatrix();
      auto projection = camera.GetProjectionMatrix();

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        lightPositions[0] += 1 * window.deltaTime * camera.Front;
      }

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        lightPositions[0] -= 1 * window.deltaTime * camera.Front;
      }
      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_UP) == GLFW_PRESS) {
        lightPositions[0].y += window.deltaTime * 0.5;
      }

      if (glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        lightPositions[0].y -= window.deltaTime * 0.5;
      }

      glBindFramebuffer(GL_FRAMEBUFFER, G_buffer);
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      gBufferShader.Use();
      gBufferShader.SetUniform("view", view);
      gBufferShader.SetUniform("projection", projection);

      for (auto &pos : objectPos) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::scale(model, glm::vec3(0.18f));
        gBufferShader.SetUniform("model", model);
        nanosuit.Draw(gBufferShader);
      }

      glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
      glClear(GL_COLOR_BUFFER_BIT);
      ssaoShader.Use();
      ssaoShader.SetUniform("projection", camera.GetProjectionMatrix());
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gPositionDepth);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, gNormal);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, noiseTexture);

      glBindVertexArray(quadVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
      glClear(GL_COLOR_BUFFER_BIT);
      ssaoBlurShader.Use();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, ssaoBuffer);
      glBindVertexArray(quadVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      const bool showSsaoBlurBuffer =
          glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_V) == GLFW_PRESS;

      if (showSsaoBlurBuffer) {
        quadShader.Use();
        quadShader.SetUniform("debugMode", 1);
      } else {
        lightingPassShader.Use();
        for (std::size_t i = 0; i < lightPositions.size(); ++i) {
          const std::string lightName = "lights[" + std::to_string(i) + "]";
          lightingPassShader.SetUniform(
              lightName + ".position",
              glm::vec3(view * glm::vec4(lightPositions[i], 1.0f)));
        }
      }
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gPositionDepth);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, gNormal);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, gColorSpec);
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, ssaoBlurBuffer);

      glBindVertexArray(quadVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);

      if (showSsaoBlurBuffer) {
        quadShader.SetUniform("debugMode", 0);
      }

      if (!showSsaoBlurBuffer) {
        glEnable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, G_buffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(
            0, 0, window.GetFrameBufferWidth(), window.GetFrameBufferHeight(),
            0, 0, window.GetFrameBufferWidth(), window.GetFrameBufferHeight(),
            GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(VAO);

        lightShader.Use();
        lightShader.SetUniform("view", camera.GetViewMatrix());
        lightShader.SetUniform("projection", camera.GetProjectionMatrix());
        for (int i : std::ranges::views::iota(0, 32)) {
          lightShader.SetUniform("lightColor", lightColors[i]);
          lightShader.SetUniform(
              "model", glm::translate(glm::mat4(1.0f), lightPositions[i]));
          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
      }
      glBindVertexArray(0);

      window.UpdateDeltaTime();

      glfwSwapBuffers(window.Get_Window_Ptr());
      glfwPollEvents();
    }

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);

  } catch (std::exception &exce) {
    std::println("{}", exce.what());
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
