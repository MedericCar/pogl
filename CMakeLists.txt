
# SETUP
cmake_minimum_required(VERSION 3.12)
project(pogl)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

include_directories(
    "src"
)

set(SRC
    "src/program.cc"
    "src/main.cc")

set(OPT_FLAGS
    "-Ofast -mtune=native -floop-parallelize-all -fgcse-sm"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS} -Wall -Wextra -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")# -fsanitize=address")

if (NOT CMAKE_BUILD_TYPE)
    message("No build type selected, default release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Test")
    message("Test Mode")
    add_executable(pogl)
    target_sources(pogl PRIVATE "tests/main.cc" ${SRC})
    target_link_libraries(pogl PRIVATE
        ${LIBRARIES} 
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLM_LIBRARIES}
        ${GLFW_LIBRARIES}
    )
else()
    add_executable(pogl)
    target_sources(pogl PRIVATE "src/main.cc" ${SRC})
    target_link_libraries(pogl PRIVATE
        ${LIBRARIES} 
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLM_LIBRARIES}
        ${GLFW_LIBRARIES}
    )
endif()
