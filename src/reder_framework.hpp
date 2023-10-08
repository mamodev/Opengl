#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "plane.hpp"
#include "cube.hpp"
#include "depthBuffer.hpp"
#include "object.hpp"
#include "material.hpp"
#include "animation.hpp"
#include "light.hpp"
#include "group.hpp"
#include "skybox.hpp"

Window * window;
Camera * camera;

void framebuffer_size_callback(GLFWwindow* _window, int _h, int _w)
{
    window->resetViewport();
}

void render_framework_init() {
  window = new Window(800, 800, "Computer Graphics");
  camera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f), -90, 0);
}
