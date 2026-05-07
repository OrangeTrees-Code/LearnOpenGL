#include <glad/glad.h>
// 111
#include <GLFW/glfw3.h>

int main() {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#pragma region FBO
  unsigned int FBO;
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
  }

  // glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDeleteFramebuffers(1, &FBO);
#pragma endregion
  unsigned int RBO;
  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);
  }