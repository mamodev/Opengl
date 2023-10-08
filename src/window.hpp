#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

 class Window
{
  public:
    GLFWwindow* window;
    glm::mat4 projection;

    bool focused = true;

    double lastFrame = 0.0f, currentFrame = 0.0f, deltaTime = 0.0f;

    Window(int width, int height, const char* title) {
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      #endif

      this->window = glfwCreateWindow(width, height, title, NULL, NULL);
      if (window == NULL)
      {
          glfwTerminate();
          fprintf(stderr, "ERROR: could not open window with GLFW3\n");
          exit(EXIT_FAILURE);
      }

      glfwMakeContextCurrent(window);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      // glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);

      if (glewInit() != GLEW_OK)
      {
        glfwTerminate();
        fprintf(stderr, "ERROR: could not start GLEW\n");
        exit(EXIT_FAILURE);
      }
      
      this->resetViewport();

      glEnable(GL_DEPTH_TEST);
      glClearColor(1,1,1,1);
    }

    inline void setWindowShouldClose(bool value) {
      glfwSetWindowShouldClose(this->window, value);
    }

    inline bool shouldClose() {
      return glfwWindowShouldClose(this->window);
    }

    inline void swapBuffers() {
      glfwSwapBuffers(this->window);
    }


    inline void setMouseToCenter () {
      if(!this->focused) return;
      int width, height;
      glfwGetWindowSize(this->window, &width, &height);
      glfwSetCursorPos(this->window, width / 2, height / 2);
    } 

    inline void pollEvents() {
      glfwPollEvents();

      if (this->isPressed(GLFW_KEY_Q)) 
        this->setWindowShouldClose(true);
      else if (this->isPressed(GLFW_KEY_ESCAPE))
        this->blur();
      else if (this->isPressed(GLFW_KEY_ENTER)) 
        this->focus();

      this->lastFrame = this->currentFrame;
      this->currentFrame = glfwGetTime();
      this->deltaTime = this->currentFrame - this->lastFrame;
    }

    static inline bool isPressed(int key) {
      return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
    }

    static inline void hideCursor() {
      glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    static inline void showCursor() {
      glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    inline void blur () {
      if(!this->focused) return;
      Window::hideCursor();
      this->focused = false;
    }

    inline void focus () {
      if(this->focused) return;
      Window::showCursor();
      this->focused = true;
      setMouseToCenter();
    }

    static inline void close() {
      glfwTerminate();
    }
    
    inline void resetViewport() {
      int FramebufferWidth, FramebufferHeight;
      glfwGetFramebufferSize(this->window, &FramebufferWidth, &FramebufferHeight);
      glViewport(0, 0, FramebufferWidth, FramebufferHeight);
      this->projection = glm::perspective(glm::radians(45.0f), (float)FramebufferWidth / (float)FramebufferHeight, 0.1f, 100.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    ~Window() {
      glfwTerminate();
    }
};

#endif // !WINDOW_H