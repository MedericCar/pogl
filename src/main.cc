#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program.hh"

#define TEST_OPENGL_ERROR()                                                             \
  do {                                                                                  \
    GLenum err = glGetError();                                                          \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR: "                               \
                                      << gluErrorString(err)                            \
                                      << " file " << __FILE__                           \
                                      << " line " << __LINE__ << std::endl;             \
  } while(0)

#define WIDTH 1024
#define HEIGHT 1024


float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Camera settings
glm::vec3 camPos(0.2f, 1.0f, 5.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);

GLuint VBO;
GLuint VAO;

void window_resize(int width, int height)
{
  glViewport(0, 0, width, height);
}

void renderCallback()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  TEST_OPENGL_ERROR();
  
  glutSwapBuffers();
}

bool initGlut(int &argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitContextVersion(4, 5);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(10, 10);
  glutCreateWindow ("Bob the Blob");
  glutDisplayFunc(renderCallback);
  glutReshapeFunc(window_resize);
  return true;
}

void initGL()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

int main(int argc, char** argv)
{
  initGlut(argc, argv);

  GLenum err = glewInit();
  if (err != GLEW_OK)
  std::cout << glewGetErrorString(err) << std::endl;

  initGL();

  std::string file_v("../src/shaders/bubble.vert.glsl");
  std::string file_f("../src/shaders/bubble.frag.glsl");
  
  pogl::Program* program = new pogl::Program(file_v, file_f);
  program->use();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  TEST_OPENGL_ERROR();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  TEST_OPENGL_ERROR();

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 modelTransposeInv = glm::transpose(glm::inverse(model));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
  program->set_matrix4("model", model);  
  program->set_matrix4("modelTransposeInv", modelTransposeInv);
  program->set_matrix4("projection", projection);  
  program->set_matrix4("view", view);  

  glm::vec3 objectColor(1.0f, 0.0f, 0.0f);
  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos(0.0f, 2.0f, -2.0f);
  program->set_vec3("objectColor", objectColor);
  program->set_vec3("lightColor", lightColor);
  program->set_vec3("lightPos", lightPos);
  program->set_vec3("viewPos", camPos);
  program->set_float("time", glutGet(GLUT_ELAPSED_TIME));
  TEST_OPENGL_ERROR();

  glutMainLoop();

  delete program;
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  TEST_OPENGL_ERROR();

  return 0;
}
