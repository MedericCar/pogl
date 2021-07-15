# POGL Final Project

Implementation of various effects on a sphere in OpenGL.

## Authors
Médéric Carriat
Xavier Gomez

## Installation

### Dependencies
- CMake
- Conan

### Commands

```bash
$ mkdir build
$ cd build
$ conan install .. -s build_type=Release --build missing
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