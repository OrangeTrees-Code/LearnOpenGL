// OpenGL loader
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
// Windowing
#include "Camera.hpp"
#include "Primitive.hpp"
#include "Shader.tpp"
#include "Texture.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <print>
#include <ranges>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), 2560, 1600);

    Window window(2560, 1600, false, camera, 0.5f, 2.0f);

    std::filesystem::path ProjRoot(PROJECT_ROOT);

    Shader PBR_Shader(ProjRoot / "src/shader/general.vert",
                      ProjRoot / "src/shader/pbr.frag");

    Shader light_Shader(ProjRoot / "src/shader/general.vert",
                        ProjRoot / "src/shader/light.frag");

    Shader cylinder2cube_Shader(ProjRoot / "src/shader/cylinder2cube.vert", ProjRoot / "src/shader/cylinder2cube.frag");

    Shader cubeMap_Shader(ProjRoot / "src/shader/showCubeMap.vert", ProjRoot / "src/shader/showCubeMap.frag");

    Shader genIrraMap_Shader(ProjRoot / "src/shader/cylinder2cube.vert", ProjRoot / "src/shader/cube2irradiance.frag");
    Primitive prim;

    const std::filesystem::path rustedIronDir =
        ProjRoot / "textures/rustediron1-alt2-bl";
    unsigned int albedoMap =
        Texture::loadTexture(rustedIronDir / "rustediron2_basecolor.png", true);
    unsigned int normalMap =
        Texture::loadTexture(rustedIronDir / "rustediron2_normal.png", false);
    unsigned int metallicMap =
        Texture::loadTexture(rustedIronDir / "rustediron2_metallic.png", false);
    unsigned int roughnessMap =
        Texture::loadTexture(rustedIronDir / "rustediron2_roughness.png", false);
    unsigned int aoMap = Texture::createWhiteTexture();

    unsigned int cylinderHDR = Texture::loadTexture_HDR(ProjRoot / "textures/newport_loft.hdr", false);

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                          512,
                          512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, captureRBO);

    unsigned int envCubeMap;
    glGenTextures(1, &envCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    for (int i : std::ranges::views::iota(0, 6)) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.f), 1.0f, 0.1f, 10.0f);
    std::vector<glm::mat4> captureViews = {glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    cylinder2cube_Shader.Use();
    cylinder2cube_Shader.SetUniform("projection", captureProjection);
    cylinder2cube_Shader.SetUniform("equirectangularMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cylinderHDR);
    window.ResetViewPort(512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (int i : std::ranges::views::iota(0, 6)) {
      cylinder2cube_Shader.SetUniform("view", captureViews[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      prim.renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (int i : std::ranges::views::iota(0, 6)) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    genIrraMap_Shader.Use();
    genIrraMap_Shader.SetUniform("environmentMap", 0);
    genIrraMap_Shader.SetUniform("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    window.ResetViewPort(32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (int i : std::ranges::views::iota(0, 6)) {
      genIrraMap_Shader.SetUniform("view", captureViews[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      prim.renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    cubeMap_Shader.Use();
    cubeMap_Shader.SetUniform("environmentMap", 0);

    PBR_Shader.Use();
    PBR_Shader.SetUniform("albedoMap", 0);
    PBR_Shader.SetUniform("normalMap", 1);
    PBR_Shader.SetUniform("metallicMap", 2);
    PBR_Shader.SetUniform("roughnessMap", 3);
    PBR_Shader.SetUniform("aoMap", 4);
    PBR_Shader.SetUniform("irradianceMap", 5);
    std::vector<glm::vec3> lightPositions = {
        glm::vec3(-10.0f, 10.0f, 10.0f),
        glm::vec3(10.0f, 10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };
    std::vector<glm::vec3> lightColors = {
        glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(300.0f, 300.0f, 300.0f)};
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_MULTISAMPLE);

    window.ResetViewPort();
    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
      window.processInput(camera, lightPositions);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDepthFunc(GL_LESS);

      PBR_Shader.Use();
      PBR_Shader.SetUniform("view", camera.GetViewMatrix());
      PBR_Shader.SetUniform("projection", camera.GetProjectionMatrix());
      PBR_Shader.SetUniform("camPos", camera.Position);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, albedoMap);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, normalMap);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, metallicMap);
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, roughnessMap);
      glActiveTexture(GL_TEXTURE4);
      glBindTexture(GL_TEXTURE_2D, aoMap);
      glActiveTexture(GL_TEXTURE5);
      glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);

      for (size_t i = 0; i < lightPositions.size(); ++i) {
        glm::vec3 newPos = lightPositions[i] +
                           glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        newPos = lightPositions[i];
        PBR_Shader.SetUniform("lights[" + std::to_string(i) + "].Position",
                              newPos);
        PBR_Shader.SetUniform("lights[" + std::to_string(i) + "].Color",
                              lightColors[i]);
      }

      for (int row = 0; row < nrRows; ++row) {
        for (int col = 0; col < nrColumns; ++col) {
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(
              model,
              glm::vec3((col - ((float)(nrColumns - 1) / 2.0f)) * spacing,
                        (row - ((float)(nrRows - 1) / 2.0f)) * spacing, 0.0f));
          PBR_Shader.SetUniform("model", model);
          PBR_Shader.SetUniform("normalMatrix",
                                glm::transpose(glm::inverse(glm::mat3(model))));
          prim.renderSphere();
        }
      }

      light_Shader.Use();
      light_Shader.SetUniform("view", camera.GetViewMatrix());
      light_Shader.SetUniform("projection", camera.GetProjectionMatrix());
      for (size_t i = 0; i < lightPositions.size(); ++i) {
        glm::vec3 newPos = lightPositions[i] +
                           glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        newPos = lightPositions[i];

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.5f));
        light_Shader.SetUniform("model", model);
        light_Shader.SetUniform("lightColor", lightColors[i]);
        prim.renderSphere();
      }

      glDepthFunc(GL_LEQUAL);
      cubeMap_Shader.Use();
      cubeMap_Shader.SetUniform("view", camera.GetViewMatrix());
      cubeMap_Shader.SetUniform("projection", camera.GetProjectionMatrix());
      glActiveTexture(GL_TEXTURE0);
      const bool visualizeIrradiance =
          glfwGetKey(window.Get_Window_Ptr(), GLFW_KEY_V) == GLFW_PRESS;
      glBindTexture(GL_TEXTURE_CUBE_MAP,
                    visualizeIrradiance ? irradianceMap : envCubeMap);
      prim.renderCube();

      window.UpdateDeltaTime();

      glfwSwapBuffers(window.Get_Window_Ptr());
      glfwPollEvents();
    }

    glBindVertexArray(0);
    glDeleteTextures(1, &albedoMap);
    glDeleteTextures(1, &normalMap);
    glDeleteTextures(1, &metallicMap);
    glDeleteTextures(1, &roughnessMap);
    glDeleteTextures(1, &aoMap);

  } catch (std::exception &exce) {
    std::println("{}", exce.what());
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
