#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
  public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath)
    {
      std::string vsCode;
      std::string fsCode;
      std::ifstream vsFile;
      std::ifstream fsFile;

      vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try
      {
        vsFile.open(vertexPath);
        fsFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vsFile.rdbuf();
        fShaderStream << fsFile.rdbuf();

        vsFile.close();
        fsFile.close();

        vsCode = vShaderStream.str();
        fsCode = fShaderStream.str();
      }
      catch (std::ifstream::failure &e)
      {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
      }
      const char *vShaderCode = vsCode.c_str();
      const char *fShaderCode = fsCode.c_str();

      unsigned int vertex, fragment;
      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      checkErrors(vertex, "VERTEX");
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
      checkErrors(fragment, "FRAGMENT");
  
      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);

      glLinkProgram(ID);
      checkErrors(ID, "PROGRAM");
      glDeleteShader(vertex);
      glDeleteShader(fragment);
    
    }

    void use()
    {
      glUseProgram(ID);
    }

    void setInt(const std::string &name, int value) const
    {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
  
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
  
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

  private:
    void checkErrors(GLuint shader, std::string type)
    {
      GLint success;
      GLchar infoLog[1024];
      if (type != "PROGRAM")
      {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
          glGetShaderInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
      }
      else
      {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
          glGetProgramInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
      }
    }
};
#endif
