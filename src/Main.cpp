// OpenGL loader
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
// Windowing
#include "Camera.hpp"
#include "Primitive.hpp"
#include "Shader.tpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <print>
#include <stdexcept>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), 2560, 1600);

    Window window(2560, 1600, false, camera, 0.5f, 2.0f);

    std::filesystem::path ProjRoot(PROJECT_ROOT);
    auto loadTexture = [](const std::filesystem::path &path,
                          bool gammaCorrection) {
      unsigned int texture;
      glGenTextures(1, &texture);

      int width, height, channels;
      unsigned char *data =
          stbi_load(path.c_str(), &width, &height, &channels, 0);
      if (data == nullptr) {
        throw std::runtime_error("Failed to load texture: " + path.string());
      }

      GLenum dataFormat = GL_RGB;
      GLenum internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
      if (channels == 1) {
        dataFormat = GL_RED;
        internalFormat = GL_RED;
      } else if (channels == 4) {
        dataFormat = GL_RGBA;
        internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
      }

      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
                   dataFormat, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
      return texture;
    };

    auto createWhiteTexture = []() {
      unsigned int texture;
      std::vector<unsigned char> pixel = {255};
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE,
                   pixel.data());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      return texture;
    };

    Shader PBR_Shader(ProjRoot / "src/general.vert", ProjRoot / "src/pbr.frag");

    Shader light_Shader(ProjRoot / "src/general.vert",
                        ProjRoot / "src/light.frag");
    Primitive prim;

    const std::filesystem::path rustedIronDir =
        ProjRoot / "textures/rustediron1-alt2-bl";
    unsigned int albedoMap =
        loadTexture(rustedIronDir / "rustediron2_basecolor.png", true);
    unsigned int normalMap =
        loadTexture(rustedIronDir / "rustediron2_normal.png", false);
    unsigned int metallicMap =
        loadTexture(rustedIronDir / "rustediron2_metallic.png", false);
    unsigned int roughnessMap =
        loadTexture(rustedIronDir / "rustediron2_roughness.png", false);
    unsigned int aoMap = createWhiteTexture();

    PBR_Shader.Use();
    PBR_Shader.SetUniform("albedoMap", 0);
    PBR_Shader.SetUniform("normalMap", 1);
    PBR_Shader.SetUniform("metallicMap", 2);
    PBR_Shader.SetUniform("roughnessMap", 3);
    PBR_Shader.SetUniform("aoMap", 4);
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

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
      window.processInput(camera, lightPositions);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        PBR_Shader.SetUniform("metallic", (float)row / (float)nrRows);
        for (int col = 0; col < nrColumns; ++col) {
          // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces
          // (roughness of 0.0) tend to look a bit off on direct lighting.
          PBR_Shader.SetUniform(
              "roughness",
              glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

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
