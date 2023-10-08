#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

float __texture_border_color[] = { 1.0, 1.0, 1.0, 1.0 };

class DepthBuffer {
  public:
    GLuint FBO;
    GLuint texture;
    int height, width;

    DepthBuffer (int height, int width) {

      this->height = height;
      this->width = width;

      glGenFramebuffers(1, &this->FBO);
      glGenTextures(1, &this->texture);
      
      glBindTexture(GL_TEXTURE_2D,  this->texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, __texture_border_color);
      glBindTexture(GL_TEXTURE_2D, 0);

      glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->texture, 0);
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    inline void startRedering () {
      glViewport(0, 0, this->width, this->height);
      glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
      glClear(GL_DEPTH_BUFFER_BIT);
    }

    inline void stopRendering () {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    inline void bindTexture (GLenum slot) {
      glActiveTexture(slot);
      glBindTexture(GL_TEXTURE_2D, this->texture);
    }

    ~DepthBuffer () {
      glDeleteFramebuffers(1, &this->FBO);
      glDeleteTextures(1, &this->texture);
    }
};

#endif