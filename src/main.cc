#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "program.hh"
#include "setup.hh"
#include "sphere.hh"

std::vector<float> vertices;
std::vector<unsigned int> indices;

// Camera settings
glm::vec3 camPos(0.0f, 0.0f, 8.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);

/*glm::vec3 camPos(0.2f, 8.0f, 0.0f);
glm::vec3 camUp(0.0f, 0.0f, 1.0f);
glm::vec3 camFront(0.0f, -1.0f, 0.0f);*/

GLuint VBO;
GLuint VAO;
GLuint EBO;  //for indices

void init_sphere()
{
  Point3 center(0, 0, 0);
  Sphere s(center, 1);

  auto data = s.generate_vertices(80, 80);
  vertices = data.first;
  indices = data.second;

  glGenVertexArrays(1, &VAO); 
  glGenBuffers(1, &VBO); 
  glGenBuffers(1, &EBO); 

  glBindVertexArray(VAO); 
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);   

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW); 

  // Vertex pos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
  glEnableVertexAttribArray(0); 

  // Vertex normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); 
  glEnableVertexAttribArray(1); 

  glBindVertexArray(0); 
}

void render(GLFWwindow* window, pogl::Program* program)
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  program->set_float("time", glfwGetTime());
  
  glBindVertexArray(VAO); 
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
  glBindVertexArray(0); 

  glfwSwapBuffers(window);
  glfwPollEvents();
}


int main(int argc, char** argv)
{
  // Setup
  GLFWwindow* window = pogl::initGLFW();

  GLenum err = glewInit();
  if (err != GLEW_OK)
    std::cout << glewGetErrorString(err) << std::endl;

  pogl::initGL();

  // Create shaders program
  std::string file_v("../src/shaders/bubble.vert.glsl");
  std::string file_f("../src/shaders/bubble.frag.glsl");
  pogl::Program* program = new pogl::Program(file_v, file_f);
  program->use();

  // Load sphere mesh to buffers
  init_sphere();

  // Load transform matrices
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 modelTransposeInv = glm::transpose(glm::inverse(model));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                          pogl::width / pogl::height,
                                          0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
  program->set_matrix4("model", model);  
  program->set_matrix4("modelTransposeInv", modelTransposeInv);
  program->set_matrix4("projection", projection);  
  program->set_matrix4("view", view);  

  // Load view information
  glm::vec3 objectColor(1.0f, 0.0f, 0.0f);
  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos(0.0f, 2.0f, 0.0f);
  program->set_vec3("objectColor", objectColor);
  program->set_vec3("lightColor", lightColor);
  program->set_vec3("lightPos", lightPos);
  program->set_vec3("viewPos", camPos);
  
  // Render loop
  while(!glfwWindowShouldClose(window))
  {
    render(window, program);
  }

  // Cleanup
  delete program;
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}
