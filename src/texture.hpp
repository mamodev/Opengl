#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <istream>  
#include "./stb_image.hpp"

class Texture {
  public: 
    GLuint textureID;
    GLenum TextureTarget;

    Texture (char ** faces) {
        this->TextureTarget = GL_TEXTURE_CUBE_MAP;
        glGenTextures(1, &this->textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

        int width, height, nrChannels;
        for (int i = 0; i < 6; i++) {

          unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);

          if(!data) {
              fprintf(stderr, "Cubemap texture failed to load at path: %s\n", faces[i]);
              exit(EXIT_FAILURE);
          }
          
          GLenum format = GL_RED;

          if (nrChannels == 3)
              format = GL_RGB;
          else if (nrChannels == 4)
              format = GL_RGBA;
          
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
                      );

          stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    Texture(const char * path) {
      this->TextureTarget = GL_TEXTURE_2D;
      glGenTextures(1, &this->textureID);

      int width, height, nrComponents;
      unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

      if(!data) {
          fprintf(stderr, "Texture failed to load at path: %s\n", path);
          stbi_image_free(data);
          exit(EXIT_FAILURE);
      }

      GLenum format = GL_RED;
      if (nrComponents == 3)
          format = GL_RGB;
      else if (nrComponents == 4)
          format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, this->textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);

      stbi_image_free(data);
    }

    inline void setParameter(GLenum pname, GLint param) {
      glBindTexture(this->TextureTarget, this->textureID);
      glTexParameteri(this->TextureTarget, pname, param);
      glBindTexture(this->TextureTarget, 0);
    }

    inline void bind(GLenum slot) {
      glActiveTexture(slot);
      glBindTexture(this->TextureTarget, this->textureID);
    }

    inline void unbind() {
      glBindTexture(this->TextureTarget, 0);
    }

    static inline void activate(GLenum texture) {
      glActiveTexture(texture);
    }
    
    ~Texture() {
      glDeleteTextures(1, &this->textureID);
    }

};

#endif  // TEXTURE_H

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);