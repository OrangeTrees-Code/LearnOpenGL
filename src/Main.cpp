// OpenGL loader
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
// Windowing
#include "Camera.hpp"
#include "Color.hpp"
#include "Global.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Shader.tpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <print>
#include <string>
#include <vector>

#include <stb_image.h>

unsigned int LoadTexture(std::filesystem::path path2Tex, int slotIndex);

int main() {
  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), 800, 600);

    Window window(800, 600, false, camera, 0.5f, 2.0f);

#pragma region Data

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.5f, -0.5f,
        -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f,
        0.0f, -1.0f, 1.0f, 1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f,
        0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f,
        -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f,
        -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, -0.5f, -0.5f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f,
        -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f,
        -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};
    std::vector<glm::vec3> objectPos{glm::vec3(0.0f, 0.0f, 0.0f)};

    std::vector<glm::vec3> lightPos{
        glm::vec3(0.7f, 0.2f, 2.0f)};

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    std::filesystem::path ProjRoot(PROJECT_ROOT);

    // stbi_set_flip_vertically_on_load(true);

    Model model_(ProjRoot / "res/backpack/backpack.obj");

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

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glBindVertexArray(0);
#pragma endregion

#pragma region Init_Texture
    std::vector<std::filesystem::path> faces = {ProjRoot / "textures/skybox/right.jpg",
                                                ProjRoot / "textures/skybox/left.jpg",
                                                ProjRoot / "textures/skybox/top.jpg",
                                                ProjRoot / "textures/skybox/bottom.jpg",
                                                ProjRoot / "textures/skybox/front.jpg",
                                                ProjRoot / "textures/skybox/back.jpg"};

    unsigned int cubeMap = Texture::LoadCubeMap(faces);

    unsigned int boxTex = Texture::LoadTexture(ProjRoot / "textures/marble.jpg", true, 1);
#pragma endregion

#pragma region Init_Shader
    Shader objShader(ProjRoot / "src/vertShader.vert",
                     ProjRoot / "src/objectFrag.frag");

    objShader.Use();
    objShader.SetUniform("material.shininess", 32.0f);

    objShader.SetUniform("dirLight.intensity", glm::vec3(1.0f, 1.0f, 1.0f));
    objShader.SetUniform("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    objShader.SetUniform("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    objShader.SetUniform("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    objShader.SetUniform("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    for (int i = 0; i < lightPos.size(); i++) {
      const std::string lightName = "pointLights[" + std::to_string(i) + "]";
      objShader.SetUniform(lightName + ".intensity",
                           glm::vec3(1.0f, 1.0f, 1.0f));
      objShader.SetUniform(lightName + ".position", lightPos[i]);
      objShader.SetUniform(lightName + ".ambient",
                           glm::vec3(0.05f, 0.05f, 0.05f));
      objShader.SetUniform(lightName + ".diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
      objShader.SetUniform(lightName + ".specular",
                           glm::vec3(1.0f, 1.0f, 1.0f));
      objShader.SetUniform(lightName + ".constant", 1.0f);
      objShader.SetUniform(lightName + ".linear", 0.09f);
      objShader.SetUniform(lightName + ".quadratic", 0.032f);
    }
    // spotLight
    objShader.SetUniform("switchSpotLight", false);
    // objShader.SetUniform("spotLight.intensity", glm::vec3(1.0f, 1.0f, 1.0f));
    // objShader.SetUniform("spotLight.position", camera.Position);
    // objShader.SetUniform("spotLight.direction", camera.Front);
    // objShader.SetUniform("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    // objShader.SetUniform("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    // objShader.SetUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    // objShader.SetUniform("spotLight.constant", 1.0f);
    // objShader.SetUniform("spotLight.linear", 0.09f);
    // objShader.SetUniform("spotLight.quadratic", 0.032f);
    // objShader.SetUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    // objShader.SetUniform("spotLight.outerCutOff",
    //                      glm::cos(glm::radians(15.0f)));

    Shader lightShader(ProjRoot / "src/vertShader.vert",
                       ProjRoot / "src/lightFrag.frag");

    lightShader.Use();
    lightShader.SetUniform("lightColor", lightColor);

    Shader skyboxShader(ProjRoot / "src/skybox.vert", ProjRoot / "src/skybox.frag");
    skyboxShader.Use();
    skyboxShader.SetUniform("cubeMap", 0);

    Shader reflectCubeShader(ProjRoot / "src/box.vert", ProjRoot / "src/reflectBox.frag");
    reflectCubeShader.Use();
    reflectCubeShader.SetUniform("skybox", 0);

    Shader refractCubeShader(ProjRoot / "src/box.vert", ProjRoot / "src/refractBox.frag");
    refractCubeShader.Use();
    refractCubeShader.SetUniform("skybox", 0);
#pragma endregion

#pragma region SetUp_Rendering
    glBindVertexArray(VAO);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glm::mat4 boxModel = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    reflectCubeShader.Use();
    reflectCubeShader.SetUniform("model", boxModel);
    boxModel = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -2.0f, -2.0f));
    refractCubeShader.Use();
    refractCubeShader.SetUniform("model", boxModel);

    float time;
#pragma endregion

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
#pragma region Before_Rendering
      window.processInput(camera);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glActiveTexture(GL_TEXTURE0);

#pragma endregion

#pragma region Rendering

      auto view = camera.GetViewMatrix();
      auto projection = camera.GetProjectionMatrix();

      objShader.Use();
      // glActiveTexture(GL_TEXTURE0);
      // glBindTexture(GL_TEXTURE_2D, textures[0]);
      // glActiveTexture(GL_TEXTURE1);
      // glBindTexture(GL_TEXTURE_2D, textures[1]);

      objShader.SetUniform("spotLight.position", camera.Position);
      objShader.SetUniform("spotLight.direction", camera.Front);

      objShader.SetUniform("viewPos", camera.Position);

      objShader.SetUniform("view", view);
      objShader.SetUniform("projection", projection);

      for (int i = 0; i < objectPos.size(); i++) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down

        objShader.SetUniform("model", model);

        model_.Draw(objShader);
      }

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

      reflectCubeShader.Use();
      reflectCubeShader.SetUniform("viewPos", camera.Position);
      reflectCubeShader.SetUniform("view", view);
      reflectCubeShader.SetUniform("projection", projection);

      glBindVertexArray(VAO);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      refractCubeShader.Use();
      refractCubeShader.SetUniform("viewPos", camera.Position);
      refractCubeShader.SetUniform("view", view);
      refractCubeShader.SetUniform("projection", projection);

      glBindVertexArray(VAO);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glDepthFunc(GL_LEQUAL);
      skyboxShader.Use();
      skyboxShader.SetUniform("view", glm::mat4(glm::mat3(view)));
      skyboxShader.SetUniform("projection", projection);
      glBindVertexArray(skyboxVAO);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glDepthFunc(GL_LESS);
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
