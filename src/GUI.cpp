#include "GUI.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

void GUI::Init(Window& window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.Get_Window_Ptr(), true);
  ImGui_ImplOpenGL3_Init("#version 460");
  glfwSetInputMode(window.Get_Window_Ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GUI::UpdateInput(Window &window, int toggleKey) {
  bool pressed = glfwGetKey(window.Get_Window_Ptr(), toggleKey) == GLFW_PRESS;
  if (pressed && !togglePressed_) {
    controlMode_ = !controlMode_;
    glfwSetInputMode(window.Get_Window_Ptr(), GLFW_CURSOR,
                     controlMode_ ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    window.ResetCursorTracking();
  }
  togglePressed_ = pressed;
}

void GUI::BeginFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

bool GUI::BeginPanel(const char *name) { return ImGui::Begin(name); }

void GUI::EndPanel() { ImGui::End(); }

bool GUI::DragFloat(const char *label, float &value, float speed, float min,
                    float max) {
  return ImGui::DragFloat(label, &value, speed, min, max, "%.2f");
}

bool GUI::DragVec3(const char *label, glm::vec3 &value, float speed, float min,
                  float max) {
  return ImGui::DragFloat3(label, glm::value_ptr(value), speed, min, max,
                           "%.2f");
}

bool GUI::Checkbox(const char *label, bool &value) {
  return ImGui::Checkbox(label, &value);
}

bool GUI::Inspect(const char *label, float &value, float speed, float min,
                  float max) {
  return DragFloat(label, value, speed, min, max);
}

bool GUI::Inspect(const char *label, int &value, int step, int fastStep) {
  return ImGui::InputInt(label, &value, step, fastStep);
}

bool GUI::Inspect(const char *label, bool &value) {
  return Checkbox(label, value);
}

bool GUI::Inspect(const char *label, glm::vec3 &value, float speed, float min,
                  float max) {
  return DragVec3(label, value, speed, min, max);
}

void GUI::Separator() { ImGui::Separator(); }

void GUI::PushID(int id) { ImGui::PushID(id); }

void GUI::PopID() { ImGui::PopID(); }

void GUI::Value(const char *label, bool value) {
  ImGui::Text("%s: %s", label, value ? "ON" : "OFF");
}

void GUI::Value(const char *label, float value) { ImGui::Text("%s: %.3f", label, value); }

void GUI::Value(const char *label, int value) { ImGui::Text("%s: %d", label, value); }

void GUI::Value(const char *label, const glm::vec3 &value) {
  ImGui::Text("%s: %.3f, %.3f, %.3f", label, value.x, value.y, value.z);
}

void GUI::DrawLightPanel(std::vector<glm::vec3> &lightPositions) {
  BeginPanel("Lights");

  for (int i = 0; i < static_cast<int>(lightPositions.size()); ++i) {
    PushID(i);
    ImGui::Text("Light %d", i);
    DragVec3("Position", lightPositions[i]);
    Separator();
    PopID();
  }

  EndPanel();
}

void GUI::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
