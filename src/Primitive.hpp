#pragma once

class Primitive {
private:
  unsigned int sphereVAO = 0;
  unsigned int indexCount;

public:
  void renderSphere();
};