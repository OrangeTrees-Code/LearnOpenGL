// OpenGL loader
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <glad/glad.h>
// Windowing
#include "Camera.hpp"
#include "Color.hpp"
#include "Global.hpp"
#include "Model.hpp"
#include "Shader.tpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include <print>
#include <vector>

#include <stb_image.h>

std::unique_ptr<std::vector<glm::mat4>> GetModelMatrices(unsigned int amount);

int main() {
  try {
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), 2560, 1600);

    Window window(800, 600, true, camera, 0.5f, 2.0f);

    std::filesystem::path ProjRoot(PROJECT_ROOT);

    auto matrices = GetModelMatrices(1000);

    Model planet_model(ProjRoot / "res/planet/planet.obj");
    Model rock_model(ProjRoot / "res/rock/rock.obj");

    unsigned int VAO, instanceVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &instanceVBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    glBufferData(GL_ARRAY_BUFFER, matrices->size() * sizeof(typename decltype(matrices)::element_type::value_type), matrices->data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    Shader shader(ProjRoot / "src/shader/demo.vert", ProjRoot / "src/shader/demo.frag");
    shader.Use();

    rock_model.SetInstanceProp<glm::mat4>(3, instanceVBO);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window.Get_Window_Ptr())) {
      window.processInput(camera);

      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.Use();
      shader.SetUniform("view", camera.GetViewMatrix());
      shader.SetUniform("projection", camera.GetProjectionMatrix());

      shader.SetUniform("isSingle", true);
      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
      model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
      shader.SetUniform("model", model);
      planet_model.Draw(shader);

      shader.SetUniform("isSingle", false);
      rock_model.DrawBatch(shader, matrices->size());

      glfwSwapBuffers(window.Get_Window_Ptr());
      glfwPollEvents();

      window.UpdateDeltaTime();
    }

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &instanceVBO);

  } catch (std::exception &exce) {
    std::println("{}", exce.what());
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

std::unique_ptr<std::vector<glm::mat4>> GetModelMatrices(unsigned int amount) {
  std::unique_ptr<std::vector<glm::mat4>> modelMatrices(new std::vector<glm::mat4>(amount));
  srand(glfwGetTime()); // 初始化随机种子
  float radius = 50.0;
  float offset = 2.5f;
  for (unsigned int i = 0; i < amount; i++) {
    glm::mat4 model(1.0f);
    // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
    float angle = glm::radians((float)i / (float)amount * 360.0f);
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // 2. 缩放：在 0.05 和 0.25f 之间缩放
    float scale = (rand() % 20) / 100.0f + 0.05;
    model = glm::scale(model, glm::vec3(scale));

    // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
    float rotAngle = glm::radians((float)(rand() % 360));
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // 4. 添加到矩阵的数组中
    modelMatrices->at(i) = model;
  }

  return modelMatrices;
}
