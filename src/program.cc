#include "program.hh"

#define TEST_OPENGL_ERROR()                                                             \
  do {                                                                                  \
    GLenum err = glGetError();                                                          \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR: "                               \
                                      << gluErrorString(err)                            \
                                      << " file " << __FILE__                           \
                                      << " line " << __LINE__ << std::endl;             \
  } while(0)

namespace pogl
{

  //Loads shader from file
  std::string load_from_file(std::string& file)
  {
    std::ifstream in(file);


    if (in.is_open())
    {
      std::string data{};
      std::string line{};

      while (getline(in, line))
	      data += line + '\n';

      in.close();

      return data;
    }    
    
    std::cout << "Unable to open file";
    return std::string{};
  }

  GLuint load_and_compile(GLenum shader_type, std::string& file)
  {
    //Create shader
    GLuint shader = glCreateShader(shader_type);

    //Load shader from file
    std::string data = load_from_file(file);
    //const GLchar* shader_array[1];//malloc(sizeof(GLchar*));
    const GLchar* source = data.c_str();

    glShaderSource(shader, 1, &source, NULL);

    //Compiling shader
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      //MaxLength includes the NULL character
      std::vector<GLchar> errorLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

      std::cout << "Out: " <<  std::string(errorLog.begin(), errorLog.end()) << '\n';
      //Exit with error
      glDeleteShader(shader);

      errx(1, "Could not compile shader: %s",  std::string(errorLog.begin(),
	    errorLog.end()).c_str());
    }

    //Return compiled shader
    return shader;
  }

  void Program::create_program()
  {
    //Create program
    id = glCreateProgram();

    //Attach shaders to program
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);

    //Link program
    glLinkProgram(id);

    GLint isLinked = 0;
    glGetProgramiv(id, GL_LINK_STATUS, (int*) &isLinked);

    //Check error
    if (isLinked == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

      //Delete program
      glDeleteProgram(id);
      
      errx(1, "Error creating program: %s", std::string(infoLog.begin(),
	    infoLog.end()).c_str());
    }

    //Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
  }

  Program::Program(std::string& vertex_shader_src,
                   std::string& fragment_shader_src)
  {
    //Load and compile shaders
    vertex_shader = load_and_compile(GL_VERTEX_SHADER, vertex_shader_src);
    fragment_shader = load_and_compile(GL_FRAGMENT_SHADER, fragment_shader_src);

    //Create program and attach shaders
    this->create_program();
  }

  void Program::use()
  {
    glUseProgram(id);
  }

  void Program::set_int(const std::string& name, int value) 
  {
    GLint64 uniformId = glGetUniformLocation(id, name.c_str());
    if (uniformId == -1)
      std::cerr << "Uniform not found : " << name << ".\n";
    else
      glUniform1i(uniformId, value);
  }

  void Program::set_float(const std::string& name, float value) 
  {
    GLint64 uniformId = glGetUniformLocation(id, name.c_str());
    if (uniformId == -1)
      std::cerr << "Uniform not found : " << name << ".\n";
    else
      glUniform1f(uniformId, value);
  }

  void Program::set_vec3(const std::string& name, const glm::vec3& value) 
  {
    GLint64 uniformId = glGetUniformLocation(id, name.c_str());
    if (uniformId == -1)
      std::cerr << "Uniform not found : " << name << ".\n";
    else
      glUniform3fv(uniformId, 1, &value[0]);
  }

  void Program::set_matrix4(const std::string& name, const glm::mat4& value) 
  {
    GLint64 uniformId = glGetUniformLocation(id, name.c_str());
    if (uniformId == -1)
      std::cerr << "Uniform not found : " << name << ".\n";
    else
      glUniformMatrix4fv(uniformId, 1, GL_FALSE, &value[0][0]);
  }

}
