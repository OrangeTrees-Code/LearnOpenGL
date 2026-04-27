#include <cstddef>
#include <glm/common.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <stb_image.h>
#include <GLFW/glfw3.h>
// #include <cmath>
#include "Window.hpp"
#include "Shader.hpp"
#include <filesystem>
#include <iostream>

int main() {
  // float vertices[] = {0.0f,
  //                     0.5f,
  //                     0.0f,
  //                     -std::sqrt(3.0f) / 4.0f,
  //                     -1.0f / 4.0f,
  //                     0.0f,
  //                     std::sqrt(3.0f) / 4.0f,
  //                     -1.0f / 4.0f,
  //                     0.0f};

  // float texCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f};
  GLFWwindow *window;
  if ((window = Window::WindowInit(true)) == NULL) {
    glfwTerminate();
    return -1;
  }


  float vertices[] = {
      //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.75f, 0.75f, // 右上
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.75f, 0.25f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.25f, // 左下
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.25f, 0.75f  // 左上
  };

  unsigned int indices[] = {0,1,2,0,2,3};

  unsigned int VAO,VBO,EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  
  glBindVertexArray(0);

  
  std::filesystem::path projRoot =
      std::filesystem::path(PROJECT_ROOT);

  Shader shader(projRoot / "src/vertShader3.vert",
                projRoot / "src/fragShader3.frag");

  unsigned int textures[2];
  glGenTextures(2, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data = stbi_load((projRoot/"textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);

  if (data != NULL) {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  glBindTexture(GL_TEXTURE_2D, textures[1]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  stbi_set_flip_vertically_on_load(true);

  data = stbi_load((projRoot / "textures/awesomeface.png").c_str(), &width,
                   &height, &nrChannels, 0);

  if (data != NULL) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }    
  stbi_image_free(data);

  shader.Use();
  shader.SetInt("texture_1", 0);
  shader.SetInt("texture_2", 1);

  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);

  float visibility = 0;

  while (glfwWindowShouldClose(window) == false) {
    Window::processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glBindTexture(GL_TEXTURE_2D, textures[0]);
    // glBindTexture(GL_TEXTURE_2D,textures[1]);

    shader.Use();
    // glBindVertexArray(VAO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      visibility += 0.01f;
      shader.SetFloat("visibility", glm::clamp(visibility,0.0f,1.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      visibility -= 0.01f;
      shader.SetFloat("visibility", glm::clamp(visibility,0.0f,1.0f));
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glBindVertexArray(0);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1,&VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}