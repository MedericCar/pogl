#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program.hh"
#include "sphere.hh"

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


float vertices_old[] = {
    -0.5f, -0.5f, 1.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, -1.0f
};

std::vector<float> vertices;
std::vector<unsigned int> indices;

// Camera settings
glm::vec3 camPos(0.0f, 0.0f, 3.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);

GLuint VBO;
GLuint VAO;
GLuint EBO;//for indices

void init_sphere()
{
  Point3 center(0, 0, 0);
  Sphere s(center, 1);

  auto data = s.generate_vertices(3, 3);
  /*auto*/ vertices = data.first;
  /*auto*/ indices = data.second;

  glGenVertexArrays(1, &VAO); TEST_OPENGL_ERROR();
  glGenBuffers(1, &VBO); TEST_OPENGL_ERROR();
  glGenBuffers(1, &EBO); TEST_OPENGL_ERROR();

  glBindVertexArray(VAO); TEST_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, VBO); TEST_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);   TEST_OPENGL_ERROR();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); TEST_OPENGL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW); TEST_OPENGL_ERROR();

  //vertex pos
  glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0); TEST_OPENGL_ERROR();

  glBindVertexArray(0); TEST_OPENGL_ERROR();
}

void window_resize(int width, int height)
{
  glViewport(0, 0,width, height);TEST_OPENGL_ERROR();
}

void renderCallback()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
  
  /*glBindVertexArray(VAO);TEST_OPENGL_ERROR();
  glDrawArrays(GL_TRIANGLES, 0, 3);TEST_OPENGL_ERROR();*/

  glBindVertexArray(VAO); TEST_OPENGL_ERROR();
  //glDrawArrays(GL_QUADS, 0, vertices.size()); TEST_OPENGL_ERROR();
  glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, &indices[0]); TEST_OPENGL_ERROR();
  glBindVertexArray(0); TEST_OPENGL_ERROR();

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
  glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
  glDepthFunc(GL_ALWAYS);TEST_OPENGL_ERROR();
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
  
  pogl::program* instance = pogl::program::make_program(file_v, file_f);
  instance->use();

  init_sphere();

  /*glGenVertexArrays(1, &VAO);TEST_OPENGL_ERROR();
  glGenBuffers(1, &VBO);TEST_OPENGL_ERROR();

  glBindVertexArray(VAO);TEST_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);TEST_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);TEST_OPENGL_ERROR();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(0);TEST_OPENGL_ERROR();*/

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
  
  glUniformMatrix4fv(glGetUniformLocation(instance->get_id(), "model"), 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(instance->get_id(), "projection"), 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(instance->get_id(), "view"), 1, GL_FALSE, &view[0][0]);

  glutMainLoop();

  delete instance;
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  return 0;
}
