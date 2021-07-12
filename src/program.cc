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

  program* program::make_program(std::string& vertex_shader_src,
    std::string& fragment_shader_src)
  {
    //Create new program
    program* instance =  new program();

    //Load and compile shaders
    instance->vertex_shader =
      load_and_compile(GL_VERTEX_SHADER, vertex_shader_src);
    instance->fragment_shader =
      load_and_compile(GL_FRAGMENT_SHADER, fragment_shader_src);

    //Create program and attach shaders
    instance->create_program();

    return instance;
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

  void program::create_program()
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

  void program::use()
  {
    glUseProgram(id);
  }

}
