#include "SSAO_Engine.hpp"
#include <cmath>
#include <glm/ext/quaternion_geometric.hpp>
#include <random>
#include <ranges>

std::unique_ptr<std::vector<glm::vec3>> SSAO_Engine::GetSampleKernel() {
  std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
  std::default_random_engine generator;
  std::unique_ptr<std::vector<glm::vec3>> ssaoKernel(new std::vector<glm::vec3>());
  for (auto i : std::ranges::views::iota(0, samples_)) {

    glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));

    sample = glm::normalize(sample);

    sample *= randomFloats(generator);

    float scale = float(i) / samples_;

    scale = std::lerp(0.1f, 1.0f, scale * scale);

    sample *= scale;

    ssaoKernel->push_back(sample);
  }

  return ssaoKernel;
}

std::unique_ptr<std::vector<glm::vec3>> SSAO_Engine::GetNoiseMatrix() {
  std::unique_ptr<std::vector<glm::vec3>> matrix(new std::vector<glm::vec3>());
  std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0

  std::default_random_engine generator;

  for (auto i : std::ranges::views::iota(0, 16)) {
    matrix->push_back(glm::vec3(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f,0.0f));
  }

  return matrix;
}