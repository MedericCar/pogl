set(OPT_FLAGS
    "-Ofast -mtune=native -floop-parallelize-all -fgcse-sm"
)


# Directories containing headers must be specified in the include_directories()
include_directories(
    "src"
)

# Searches for all .cc files
set(SRC
    "src/camera.cc"
    "src/matrix.cc"
    "src/vector3.cc"
    "src/program.cc"
    "src/tp1.cc")


# SETUP
cmake_minimum_required(VERSION 3.12)
project(tp1)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS} -Wall -Werror -Wextra -pedantic -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")# -fsanitize=address")

if (NOT CMAKE_BUILD_TYPE)
    message("No build type selected, default release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Test")
    message("Test Mode")
    add_executable(tp1)
    target_sources(tp1 PRIVATE "tests/main.cc" ${SRC})
    target_link_libraries(tp1 PRIVATE ${LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
else()
    add_executable(tp1)
    target_sources(tp1 PRIVATE "src/tp1.cc" ${SRC})
    target_link_libraries(tp1 PRIVATE ${LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
endif()
