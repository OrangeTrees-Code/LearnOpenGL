#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <linux/limits.h>
// #include <print>

const char *vertexShaderSource =
    "#version 460 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
    "}\0";

const char *fragmentShaderSource = "#version 460 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f,1.0f,1.0f,1.0f);\n"
                                   "}\0";
float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

float twoTriangles[] = {0.0f,
                        1.0f,
                        0.0f,
                        std::sqrt(3.0f) / 2,
                        -1 / 2.0f,
                        0.0f,
                        -std::sqrt(3.0f) / 2,
                        -1 / 2.0f,
                        0.0f,
                        0.0f,
                        -1.0f,
                        0.0f,
                        std::sqrt(3.0f) / 2,
                        1 / 2.0f,
                        0.0f,
                        -std::sqrt(3.0f) / 2,
                        1 / 2.0f,
                        0.0f};

float vertices2[] = {0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                     -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};

unsigned int indices[] = {

    0, 1, 3, 1, 2, 3};

int success;
char infoLog[512];

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

GLFWwindow *WindowInit() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Suki", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    return NULL;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return NULL;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  return window;
}

unsigned int InitVAO() {
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  return VAO;
}

unsigned int InitVBO() {
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(twoTriangles), twoTriangles,
               GL_STATIC_DRAW);
  return VBO;
}

unsigned int InitEBO() {
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  return EBO;
}

unsigned int InitVertexShader() {
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return vertexShader;
}

unsigned int InitFragmentShader() {
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return fragmentShader;
}

unsigned int InitShaderProgram(unsigned int vertexShader,
                               unsigned int fragmentShader) {
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  return shaderProgram;
}

int main() {

  GLFWwindow *window;
  if ((window = WindowInit()) == NULL) {
    glfwTerminate();
    return -1;
  }

  unsigned int VAO = InitVAO();

  unsigned int VBO = InitVBO();

  unsigned int EBO = InitEBO();

  unsigned int vertexShader = InitVertexShader();

  unsigned int fragmentShader = InitFragmentShader();

  unsigned int shaderProgram = InitShaderProgram(vertexShader, fragmentShader);

  glUseProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glfwTerminate();
  return 0;
}