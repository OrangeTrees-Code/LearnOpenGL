#pragma once

#include <glm/ext/vector_float3.hpp>
#include <memory>
class SSAO_Engine {
public:
  SSAO_Engine(int samples) : samples_(samples) {};

  std::unique_ptr<std::vector<glm::vec3>> GetSampleKernel();

  std::unique_ptr<std::vector<glm::vec3>> GetNoiseMatrix();
private:    
  const int samples_;
};