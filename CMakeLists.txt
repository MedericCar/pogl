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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_FLAGS} -Wall -Werror -Wextra -pedantic -fopenmp -lGL -lGLU -lglut -lGLEW")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")# -fsanitize=address")

if (NOT CMAKE_BUILD_TYPE)
    message("No build type selected, default release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Test")
    message("Test Mode")
    add_executable(tp1)
    target_sources(tp1 PRIVATE "tests/main.cc" ${SRC})
    target_link_libraries(tp1 PRIVATE ${LIBRARIES})
else()
    add_executable(tp1)
    target_sources(tp1 PRIVATE "src/tp1.cc" ${SRC})
    target_link_libraries(tp1 PRIVATE ${LIBRARIES})
endif()
