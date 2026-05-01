cmake_minimum_required(VERSION 3.15)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== 查找 OpenGL =====
find_package(OpenGL REQUIRED)

# ===== 查找 GLFW =====
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# ===== 可执行文件 =====
add_executable(app
    src/Window.cpp
    src/Shader.cpp
    src/glad.c
    src/Camera.cpp
    src/Cursor.cpp
    src/Global.hpp
    src/Main.cpp
    src/Color.cpp
)

# ===== 头文件目录 =====
target_include_directories(app PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    src
)

# ===== 链接库 =====
target_link_libraries(app
    ${GLFW_LIBRARIES}
    OpenGL::GL
)

# ===== Linux 额外依赖（有些环境需要）=====
if(UNIX)
    target_link_libraries(app dl pthread)
endif()

# ===== 把 shader 文件复制到运行目录 =====
# file(COPY src/fragShader3.frag DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY src/vertShader3.vert DESTINATION ${CMAKE_BINARY_DIR})

target_compile_definitions(app PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)