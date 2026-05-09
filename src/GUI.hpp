#pragma once

#include "Window.hpp"
#include <glm/ext/vector_float3.hpp>
#include <vector>

class GUI {
public:
  static GUI &Instance() {
    static GUI instance;
    return instance;
  }

  void Init(Window &window);
  void UpdateInput(Window &window, int toggleKey = GLFW_KEY_F1);
  void BeginFrame();
  bool BeginPanel(const char *name);
  void EndPanel();
  bool DragFloat(const char *label, float &value, float speed = 0.05f,
                 float min = -20.0f, float max = 20.0f);
  bool DragVec3(const char *label, glm::vec3 &value, float speed = 0.05f,
                float min = -20.0f, float max = 20.0f);
  bool Checkbox(const char *label, bool &value);
  bool Inspect(const char *label, float &value, float speed = 0.05f,
               float min = -20.0f, float max = 20.0f);
  bool Inspect(const char *label, int &value, int step = 1, int fastStep = 10);
  bool Inspect(const char *label, bool &value);
  bool Inspect(const char *label, glm::vec3 &value, float speed = 0.05f,
               float min = -20.0f, float max = 20.0f);
  void Separator();
  void PushID(int id);
  void PopID();
  void Value(const char *label, bool value);
  void Value(const char *label, float value);
  void Value(const char *label, int value);
  void Value(const char *label, const glm::vec3 &value);
  void DrawLightPanel(std::vector<glm::vec3> &lightPositions);
  void Render();
  void Shutdown();
  bool IsControlMode() const { return controlMode_; }

private:
  GUI() = default;
  ~GUI() = default;

  bool controlMode_ = false;
  bool togglePressed_ = false;
};
