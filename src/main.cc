#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <imgui.h>
#include "../bindings/imgui_impl_opengl3.h"
#include "../bindings/imgui_impl_glfw.h"

#include "program.hh"
#include "setup.hh"
#include "sphere.hh"

#define TEST_OPENGL_ERROR()                                                             \
  do {                                                                                  \
    GLenum err = glGetError();                                                          \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR: "                               \
                                      << gluErrorString(err)                            \
                                      << " file " << __FILE__                           \
                                      << " line " << __LINE__ << std::endl;             \
  } while(0)

std::vector<float> vertices;
std::vector<unsigned int> indices;

// Camera settings
glm::vec3 camPos(0.0f, 0.0f, 5.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);

GLuint VBO;
GLuint VAO;
GLuint EBO;  //for indices
GLuint texture;

void init_sphere()
{
  Point3 center(0, 0, 0);
  Sphere s(center, 1);

  auto data = s.generate_vertices(500, 500);
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

void render_blob(GLFWwindow* window, pogl::Program* program)
{
  program->use();
  static float frequency = 0.03;
  ImGui::SliderFloat("frequency", &frequency, 0.001, 0.1);
  static float amplitude = 0.3;
  ImGui::SliderFloat("amplitude", &amplitude, 0.01, 1);
  static glm::vec4 color1(1.0f, 0.55f, 0.0f, 1.0f);
  ImGui::ColorEdit3("color1", &color1[0]);
  static glm::vec4 color2(0.6157f, 0.1216f, 0.0f, 1.0f);
  ImGui::ColorEdit3("color2", &color2[0]);
  ImGui::End();

  program->set_float("freq", frequency);
  program->set_float("amplitude", amplitude);
  program->set_vec4("color1", color1);
  program->set_vec4("color2", color2);
  program->set_float("time", glfwGetTime());
}

void render(GLFWwindow* window, pogl::Program* program)
{
  glfwPollEvents();
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Parameters");
  const char* effects[3] = { "Wave", "Blob", "Morphing"};
  static int curr_effect = 1;
  ImGui::ListBox("Effects", &curr_effect, effects, 3, 2);
  ImGui::BeginChild("Effects");
  if (curr_effect == 0)
  {
  }
  else if (curr_effect == 1)
  {
    render_blob(window, program);
  }
  else if (curr_effect == 2)
  {
  }
  ImGui::EndChild();
  ImGui::End();

  glBindVertexArray(VAO); 
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
  glBindVertexArray(0); 

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  
  glfwSwapBuffers(window);
}

void load_transform_matrices(pogl::Program* program)
{
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
}

void load_scene(pogl::Program* program)
{
  glm::vec3 objectColor(1.0f, 0.0f, 0.0f);
  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos(0.0f, 2.0f, 2.0f);
  program->set_vec3("objectColor", objectColor);
  program->set_vec3("lightColor", lightColor);
  program->set_vec3("lightPos", lightPos);
  program->set_vec3("viewPos", camPos);
}


int main(int argc, char** argv)
{
  // Setup
  GLFWwindow* window = pogl::initGLFW();
  pogl::init_ImGui(window);

  GLenum err = glewInit();
  if (err != GLEW_OK)
    std::cout << glewGetErrorString(err) << std::endl;

  pogl::initGL();

  // Create shaders program
  std::string file_v("../src/shaders/bubble.vert.glsl");
  std::string file_f("../src/shaders/bubble.frag.glsl");
  pogl::Program* blob_program = new pogl::Program(file_v, file_f);

  // Load sphere mesh to buffers
  init_sphere();

  // Set the various uniforms in the programs
  blob_program->use();
  load_transform_matrices(blob_program);
  load_scene(blob_program);
  
  // Render loop
  while(!glfwWindowShouldClose(window))
  {
    render(window, blob_program);
  }

  // Cleanup
  delete blob_program;
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
    

  glfwTerminate();
  return 0;
}
