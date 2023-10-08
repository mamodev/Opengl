#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./shader.hpp"
#include "./window.hpp"

#define SPEED 3.0f
#define GRAVITY 15.0f
#define MOUSE_SENSITIVITY 0.06f
glm::vec3 __camera_up = glm::vec3(0.0f, 1.0f, 0.0f);


class Camera {

  public:

    bool isJumping;
    float jumpSpeed;
    float jumpingY;


    glm::vec3 position;
    float yaw, pitch;

    // computed
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 front;

    glm::mat4 viewMatrix;

    Camera (glm::vec3 pos, float yaw, float pitch) {
      this->yaw = yaw;
      this->pitch = pitch;
      this->jumpSpeed = 0.0f;
      this->isJumping = false;
      this->jumpingY = 0.0f;
      this->position = pos;
      updateVectors();
    }

    inline void activate (Shader & shader) {
      shader.setMat4("view", viewMatrix);
      shader.setVec3("viewPos", this->position);
    }

    void update (Window & window) {

      float xMov = 0, yMov = 0 , zMov = 0;
      float movementSpeed = window.deltaTime * SPEED;
      if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS) {
        xMov += front.x * movementSpeed;
        zMov += front.z * movementSpeed;
      }

      if (glfwGetKey(window.window, GLFW_KEY_S) == GLFW_PRESS) {
        xMov -= front.x * movementSpeed;
        zMov -= front.z * movementSpeed;
      }

      if (glfwGetKey(window.window, GLFW_KEY_A) == GLFW_PRESS) {
        xMov -= right.x * movementSpeed;
        zMov -= right.z * movementSpeed;
      }

      if (glfwGetKey(window.window, GLFW_KEY_D) == GLFW_PRESS) {
        xMov += right.x * movementSpeed;
        zMov += right.z * movementSpeed;
      }

      if (glfwGetKey(window.window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping) {
        this->isJumping = true;
        this->jumpSpeed = 3.0f;
        this->jumpingY = this->position.y;
      }

      if (this->isJumping) {

        yMov += this->jumpSpeed * window.deltaTime;

        if(this->position.y + yMov <= this->jumpingY) {
          this->isJumping = false;
          yMov = this->jumpingY - this->position.y;
        }

        this->jumpSpeed -= GRAVITY * window.deltaTime;
  
      }

      // update yaw and pitch
      double xpos, ypos;
      int height, width;
      glfwGetCursorPos(window.window, &xpos, &ypos);
      glfwGetWindowSize(window.window, &width, &height);
      // glfwSetCursorPos(window.window, width / 2, height / 2);

      float xoffset = xpos - width/2;
      float yoffset = height/2 - ypos;

      xoffset *= MOUSE_SENSITIVITY;
      yoffset *= MOUSE_SENSITIVITY;

      this->yaw += xoffset;
      this->pitch += yoffset;

      // constrain pitch and yaw values to prevent screen flip
      if (this->pitch > 89.0f)
        this->pitch = 89.0f;

      if (this->pitch < -89.0f)
        this->pitch = -89.0f;
      
      if(this->yaw > 360.0f)
        this->yaw -= 360.0f;

      if(this->yaw < -360.0f)
        this->yaw += 360.0f;

      this->position += glm::vec3(xMov, yMov, zMov);
      updateVectors();
      window.setMouseToCenter();

    }

  private:
    void updateVectors () {
   
      this->front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      this->front.y = sin(glm::radians(pitch));
      this->front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      this->front = glm::normalize(front);

      this->right = glm::normalize(glm::cross(front, __camera_up));
      this->up = glm::normalize(glm::cross(right, front));

      viewMatrix = glm::lookAt(position, position + front, up);
    }
   
};

#endif
