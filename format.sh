#!/bin/bash
find ./src -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.tpp" \) \
  -exec clang-format -i {} +