#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace pogl
{
  const float width = 1024;
  const float height = 1024;

  void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  // From https://gist.github.com/liam-middlebrook/c52b069e4be2d87a6d2f
  void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *msg, const void *data);
  
  GLFWwindow* initGLFW();

  void initGL();
}

