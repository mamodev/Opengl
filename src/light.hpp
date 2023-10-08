#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.hpp"
#include "depthBuffer.hpp"
#include "shader.hpp"

#define DEPTH_TEXTURE_OFFSET 2

class Light {

  public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    glm::mat4 lightSpaceMatrix;
    DepthBuffer * depthBuffer;

    Light (glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {

      this->ambient = ambient;
      this->diffuse = diffuse;
      this->specular = specular;

      this->constant = constant;
      this->linear = linear;
      this->quadratic = quadratic;

      this->depthBuffer = new DepthBuffer(3000, 3000);
    
      this->setPosition(position);
    }

    ~Light () {
      delete depthBuffer;
    }

    void renderShadow (Shader & shader, Drawable & scene) {
      glClear(GL_DEPTH_BUFFER_BIT);
      depthBuffer->startRedering();
      shader.use();
      shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
      scene.draw(shader, NULL);
      depthBuffer->stopRendering();
    }

    void setPosition (glm::vec3 position) {
      this->position = position;

      glm::mat4 lightProjection, lightView;
      float near_plane = 1.0f, far_plane = 10.0f;

      lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
      lightView = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
      this->lightSpaceMatrix = lightProjection * lightView;
    }

    void bind (Shader shader, int index) {
      depthBuffer->bindTexture(GL_TEXTURE0 + index + DEPTH_TEXTURE_OFFSET);

      shader.setVec3("lights[" + std::to_string(index) + "].position", position);
      shader.setVec3("lights[" + std::to_string(index) + "].ambient", ambient);
      shader.setVec3("lights[" + std::to_string(index) + "].diffuse", diffuse);
      shader.setVec3("lights[" + std::to_string(index) + "].specular", specular);
      shader.setFloat("lights[" + std::to_string(index) + "].constant", constant);
      shader.setFloat("lights[" + std::to_string(index) + "].linear", linear);
      shader.setFloat("lights[" + std::to_string(index) + "].quadratic", quadratic);
      shader.setInt("lights[" + std::to_string(index) + "].depthMap", index + DEPTH_TEXTURE_OFFSET);
      shader.setMat4("lightSpaceMatrix[" + std::to_string(index) + "]" , lightSpaceMatrix);
    }
};

#endif // LIGHT_H