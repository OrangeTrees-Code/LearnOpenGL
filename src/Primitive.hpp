#pragma once

class Primitive {
private:
  unsigned int sphereVAO = 0;
  unsigned int indexCount;

  unsigned int cubeVAO = 0;
  unsigned int cubeVBO = 0;

public:
  void renderSphere();

  void renderCube();
};