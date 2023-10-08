#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <GL/glew.h>
#include "texture.hpp"
#include "cube.hpp"
#include "shader.hpp"

const char * faces [] = {
  "%s/right.jpg",
  "%s/left.jpg",
  "%s/top.jpg",
  "%s/bottom.jpg",
  "%s/front.jpg",
  "%s/back.jpg"
};


class SkyBox {
  public :

    glm::vec3 lightDir;
    glm::vec3 lightColor;
    glm::vec3 lightAmbient;

    Texture * cubeMap;
    Cube * cube;
    Shader * shader;

    SkyBox (const char * path, Shader * shader, glm::vec3 lightDir, glm::vec3 lightColor, glm::vec3 lightAmbient) {

      this->lightDir = lightDir;
      this->lightColor = lightColor;
      this->lightAmbient = lightAmbient;

      char * faceFiles [6];

      for (int i = 0; i < 6; i++) {
        faceFiles[i] = (char *) malloc(strlen(faces[i]) + strlen(path) + 1);
        sprintf(faceFiles[i], faces[i], path);
      }

      this->cubeMap = new Texture(faceFiles);

      for (int i = 0; i < 6; i++) {
        free(faceFiles[i]);
      }

      cube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

      this->shader = shader;

    }

    ~SkyBox () {
      delete cubeMap;
      delete cube;
    }
    
    void bind (Shader& shader) {
      shader.setInt("skybox", 1);
      this->cubeMap->bind(GL_TEXTURE1);
      shader.setVec3("skyLightDir", this->lightDir);
      shader.setVec3("skyLightColor", this->lightColor);
      shader.setVec3("skyLightAmbient", this->lightAmbient);
    }

    void renderSkyBox (glm::mat4 projection, glm::mat4  view) {
      glDepthMask(GL_FALSE);
      glDepthFunc(GL_LEQUAL);
      glDisable(GL_CULL_FACE);

      this->shader->use();
      this->shader->setMat4("view", glm::mat4(glm::mat3(view)));
      this->shader->setMat4("projection", projection);

      this->cubeMap->bind(GL_TEXTURE0);
      this->cube->draw(*this->shader, NULL);

      glEnable(GL_CULL_FACE);
      glDepthMask(GL_TRUE);
      glDepthFunc(GL_LESS);
    }

};

#endif // SKY_BOX_H