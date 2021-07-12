#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "program.hh"

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutSwapBuffers();
}

bool initGlut(int &argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(4, 5);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(1024, 1024);
  glutInitWindowPosition(10, 10);
  glutCreateWindow ("Test OpenGL−POGL");
  glutDisplayFunc(display);
  return true;
}

/*bool initGlew()
{
  return (glewInit() ==GLEW_OK);
}*/

void init()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHT1);
}

int main(int argc, char** argv)
{

  initGlut(argc, argv);

  GLenum err = glewInit();
  if (err != GLEW_OK)
    std::cout << glewGetErrorString(err) << std::endl;

  init();

  std::string file_v("../src/shaders/bubble.vert.glsl");
  std::string file_f("../src/shaders/bubble.frag.glsl");
  
  program::program* instance = program::program::make_program(file_v, file_f);

  instance->use();

  glutMainLoop();

  delete instance;

  return 0;
}
