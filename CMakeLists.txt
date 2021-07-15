cmake_minimum_required(VERSION 3.12)
project(pogl)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup()

set(SRC
    "src/main.cc"
    "src/program.cc"
    "src/setup.cc"
    "src/sphere.cc"
    "src/vector3.cc"
)

add_executable(pogl
    ${SRC}
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_opengl3.cpp
    bindings/imgui_impl_glfw.h
    bindings/imgui_impl_opengl3.h
)

target_compile_definitions(pogl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)

conan_target_link_libraries(pogl)