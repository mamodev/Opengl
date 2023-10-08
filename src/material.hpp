#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.hpp"
#include "shader.hpp"

class Material {
  public:
    glm::vec3 ambient;
    glm::vec3 specular;
    float shininess;
    float opacity;
    float reflectivity;
    Texture * diffuseTexture;

    Material (glm::vec3 ambient, glm::vec3 specular, float shininess, float opacity, float reflectivity, Texture * diffuseTexture) {
      this->ambient = ambient;
      this->specular = specular;
      this->shininess = shininess;
      this->opacity = opacity;
      this->diffuseTexture = diffuseTexture;
      this->reflectivity = reflectivity;
    }

    inline void bind (Shader& shader) {

      if(this->diffuseTexture != NULL) {
        this->diffuseTexture->bind(GL_TEXTURE0);
      } else {
        glBindTexture(GL_TEXTURE_2D, 0);
      }

      // set material struct uniforms
      glUniform3fv(glGetUniformLocation(shader.ID, "material.ambient"), 1, glm::value_ptr(this->ambient));
      glUniform3fv(glGetUniformLocation(shader.ID, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f)));
      glUniform3fv(glGetUniformLocation(shader.ID, "material.specular"), 1, glm::value_ptr(this->specular));
      glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), this->shininess);
      glUniform1f(glGetUniformLocation(shader.ID, "material.opacity"), this->opacity);
      glUniform1i(glGetUniformLocation(shader.ID, "material.hasTexture"), this->diffuseTexture != NULL);
      glUniform1f(glGetUniformLocation(shader.ID, "material.reflectivity"), this->reflectivity);


      if (this->opacity != 1) {
        glDisable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
      } else {
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
      }
    }
};



#endif // MATERIAL_H