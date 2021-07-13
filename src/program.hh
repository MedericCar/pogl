#pragma once

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <err.h>
#include <glm/gtc/type_ptr.hpp>

namespace pogl
{

class Program
{
  public:
    Program(std::string& vertex_shader_src, std::string& fragment_shader_src);
    ~Program() { glDeleteProgram(id); }

    char* get_log();
    void use();
    GLuint get_id() const { return id; }

    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vec3(const std::string& name, const glm::vec3& value);
    void set_matrix4(const std::string& name, const glm::mat4& value);

  private:
    void create_program();
    
    //Program id
    GLuint id;

    GLuint vertex_shader;
    GLuint fragment_shader;
};

std::string load_from_file(std::string& file);

GLuint load_and_compile(GLenum shader_type, std::string& file);

}
