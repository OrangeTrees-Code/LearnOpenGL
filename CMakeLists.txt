cmake_minimum_required(VERSION 3.15)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== 查找 OpenGL =====
find_package(OpenGL REQUIRED)

# ===== 查找 GLFW =====
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# ===== 查找 Assimp =====
find_package(assimp REQUIRED)

# ===== 可执行文件 =====
add_executable(app
    src/Window.cpp
    src/Shader.cpp
    src/glad.c
    src/stb_image.cpp
    src/Camera.cpp
    src/Cursor.cpp
    src/Global.hpp
    src/Main.cpp
    src/Color.cpp
    src/Mesh.cpp
    src/Model.cpp
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
    src/GUI.cpp
)

# ===== 头文件目录 =====
target_include_directories(app PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/imgui
    ${CMAKE_SOURCE_DIR}/include/imgui/backends
    src
)

# ===== 链接库 =====
target_link_libraries(app
    ${GLFW_LIBRARIES}
    OpenGL::GL
    assimp   # 👈 关键
)

# ===== Linux 额外依赖 =====
if(UNIX)
    target_link_libraries(app dl pthread)
endif()

# ===== 定义宏 =====
target_compile_definitions(app PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
