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
//FRONT
glm::vec3 camPos(0.0f, 0.0f, 8.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);

//UP
/*glm::vec3 camPos(0.0f, 8.0f, 0.0f);
glm::vec3 camUp(0.0f, 0.0f, 1.0f);
glm::vec3 camFront(0.0f, -1.0f, 0.0f);*/

//RIGHT
/*glm::vec3 camPos(8.0f, 0.0f, 0.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(-1.0f, 0.0f, 0.0f);*/

//LEFT
/*glm::vec3 camPos(-8.0f, 0.0f, 0.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camFront(1.0f, 0.0f, 0.0f);*/

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

void render_wave(GLFWwindow* window, pogl::Program* program)
{
  program->use();

  program->set_float("time", glfwGetTime());
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

void render_morphing(GLFWwindow* window, pogl::Program* program)
{
  program->use();

  program->set_float("time", glfwGetTime());
}

void render_laser(GLFWwindow* window, pogl::Program* program)
{
  program->use();

  program->set_float("time", glfwGetTime());
}

void render_random(GLFWwindow* window, pogl::Program* program)
{
  program->use();

  program->set_float("time", glfwGetTime());
}

void render(GLFWwindow* window, std::vector<pogl::Program*> programs)
{
  glfwPollEvents();
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Parameters");
  const char* effects[5] = { "Wave", "Blob", "Morphing", "Laser", "Random"};
  static int prev = -1;
  static int curr_effect = 0;
  ImGui::ListBox("Effects", &curr_effect, effects, 5, 5);
  ImGui::BeginChild("Effects");

  //Reset time
  if (prev != curr_effect)
  {
    glfwSetTime(0);
    prev = curr_effect;
  }

  switch (curr_effect)
  {
  case 0:
    render_wave(window, programs[0]);
    break;
  case 1:
    render_blob(window, programs[1]);
    break;
  case 2:
    render_morphing(window, programs[2]);
    break;
  case 3:
    render_laser(window, programs[3]);
    break;
  case 4:
    render_random(window, programs[4]);
    break;
  default:
    break;
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
  std::string file_v1("../src/shaders/bubble.vert.glsl");
  std::string file_f1("../src/shaders/bubble.frag.glsl");
  pogl::Program* blob_program = new pogl::Program(file_v1, file_f1);

  std::string file_v2("../src/shaders/laser.vert.glsl");
  std::string file_f2("../src/shaders/laser.frag.glsl");
  pogl::Program* laser_program = new pogl::Program(file_v2, file_f2);

  std::string file_v3("../src/shaders/morph.vert.glsl");
  std::string file_f3("../src/shaders/morph.frag.glsl");
  pogl::Program* morph_program = new pogl::Program(file_v3, file_f3);

  std::string file_v4("../src/shaders/random.vert.glsl");
  std::string file_f4("../src/shaders/random.frag.glsl");
  pogl::Program* random_program = new pogl::Program(file_v4, file_f4);

  std::string file_v5("../src/shaders/wave.vert.glsl");
  std::string file_f5("../src/shaders/wave.frag.glsl");
  pogl::Program* wave_program = new pogl::Program(file_v5, file_f5);

  // Load sphere mesh to buffers
  init_sphere();

  // Set the various uniforms in the programs
  blob_program->use();
  load_transform_matrices(blob_program);
  load_scene(blob_program);

  laser_program->use();
  load_transform_matrices(laser_program);
  load_scene(laser_program);

  morph_program->use();
  load_transform_matrices(morph_program);
  load_scene(morph_program);

  random_program->use();
  load_transform_matrices(random_program);
  load_scene(random_program);

  wave_program->use();
  load_transform_matrices(wave_program);
  load_scene(wave_program);
  
  // Render loop
  while(!glfwWindowShouldClose(window))
  {
    std::vector<pogl::Program*> programs = {
      wave_program, blob_program, morph_program, laser_program, random_program
    };
    render(window, programs);
  }

  // Cleanup
  delete blob_program;
  delete laser_program;
  delete morph_program;
  delete random_program;
  delete wave_program;
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
    
  glfwTerminate();
  return 0;
}
