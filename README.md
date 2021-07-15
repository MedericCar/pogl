# POGL Final Project

Implementation of various effects on a sphere in OpenGL.

## Installation

### Dependencies
- CMake
- Conan

### Commands

```bash
$ mkdir build
$ cd build
$ conan install .. -s build_type=Release 
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
$ ./pogl
```

## Usage
Once the window is opened, a menu should guide you through the different effects.

## Implemented
- Perlin noise deformation
- Morphing
- Wave