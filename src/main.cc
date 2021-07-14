#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

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
glm::vec3 camPos(0.2f, 0.0f, 8.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);

/*glm::vec3 camPos(0.2f, 8.0f, 0.0f);
glm::vec3 camUp(0.0f, 0.0f, 1.0f);
glm::vec3 camFront(0.0f, -1.0f, 0.0f);*/

GLuint VBO;
GLuint VAO;
GLuint EBO;//for indices

void init_sphere()
{
  Point3 center(0, 0, 0);
  Sphere s(center, 1);

  auto data = s.generate_vertices(80, 80);
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();

  //vertex normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(1); TEST_OPENGL_ERROR();

  glBindVertexArray(0); TEST_OPENGL_ERROR();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

GLFWwindow* initGLFW()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //glfwSetCursorPosCallback(window, mouse_callback); 

  return window;
}

void initGL()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void render(GLFWwindow* window, pogl::Program* program)
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  //program->set_float("time", glfwGetTime());
  
  /*glBindVertexArray(VAO);TEST_OPENGL_ERROR();
  glDrawArrays(GL_TRIANGLES, 0, 3);TEST_OPENGL_ERROR();*/

  glBindVertexArray(VAO); TEST_OPENGL_ERROR();
  //glDrawArrays(GL_QUADS, 0, vertices.size()); TEST_OPENGL_ERROR();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, /*&indices[0]*/0); TEST_OPENGL_ERROR();
  //glDrawArrays(GL_POINTS, 0, vertices.size());TEST_OPENGL_ERROR();
  glBindVertexArray(0); TEST_OPENGL_ERROR();

  glfwSwapBuffers(window);
  glfwPollEvents();
}


int main(int argc, char** argv)
{
  GLFWwindow* window = initGLFW();

  GLenum err = glewInit();
  if (err != GLEW_OK)
    std::cout << glewGetErrorString(err) << std::endl;

  initGL();

  std::string file_v("../src/shaders/bubble.vert.glsl");
  std::string file_f("../src/shaders/bubble.frag.glsl");
  
  pogl::Program* program = new pogl::Program(file_v, file_f);
  program->use();

  init_sphere();

  /*glGenVertexArrays(1, &VAO);TEST_OPENGL_ERROR();
  glGenBuffers(1, &VBO);TEST_OPENGL_ERROR();
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  TEST_OPENGL_ERROR();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(0);TEST_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  TEST_OPENGL_ERROR();
  */

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 modelTransposeInv = glm::transpose(glm::inverse(model));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                          (float)WIDTH / (float)HEIGHT,
                                          0.1f, 100.0f);
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
  TEST_OPENGL_ERROR();

  while(!glfwWindowShouldClose(window))
  {
    render(window, program);
  }

  delete program;
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  TEST_OPENGL_ERROR();

  glfwTerminate();
  return 0;
}
