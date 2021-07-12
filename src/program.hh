#pragma once

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <err.h>

namespace program
{

class program
{
  public:
    program() = default;

    ~program()
    {
      glDetachShader(id, vertex_shader);
      glDetachShader(id, fragment_shader);

      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
    }

    static program *make_program(std::string& vertex_shader_src,
	std::string& fragment_shader_src);

    char* get_log();

    bool is_ready();

    void use();

  private:
    //static program* instance;
    void create_program();
    
    //Program id
    GLuint id;

    GLuint vertex_shader;
    GLuint fragment_shader;
};

std::string load_from_file(std::string& file);

GLuint load_and_compile(GLenum shader_type, std::string& file);

}
