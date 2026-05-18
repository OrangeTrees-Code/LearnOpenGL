#pragma once

#include <filesystem>
class Texture {
public:
  static unsigned int loadTexture(const std::filesystem::path &path,
                                  bool gammaCorrection);
  static unsigned int createWhiteTexture();
  static unsigned int loadTexture_HDR(const std::filesystem::path &path,
                                      bool gammaCorrection);
};