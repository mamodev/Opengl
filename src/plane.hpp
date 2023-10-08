#ifndef PLANE_H 
#define PLANE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./mesh.hpp"
#include "./shader.hpp"

class Plane : public Drawable {
  public: 
    Mesh * mesh;
    GLfloat vertex[8 * 6];

    Plane (glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, float textureRepeat = 1.0f) {
      glm::vec3 normal = glm::normalize(glm::cross(pos2 - pos1, pos4 - pos1));
      
      this->vertex[0] = pos1.x;
      this->vertex[1] = pos1.y;
      this->vertex[2] = pos1.z;
      this->vertex[3] = normal.x;
      this->vertex[4] = normal.y;
      this->vertex[5] = normal.z;
      this->vertex[6] = 0.0f * textureRepeat;
      this->vertex[7] = 1.0f * textureRepeat;

      this->vertex[8] = pos2.x;
      this->vertex[9] = pos2.y;
      this->vertex[10] = pos2.z;
      this->vertex[11] = normal.x;
      this->vertex[12] = normal.y;
      this->vertex[13] = normal.z;
      this->vertex[14] = 1.0f * textureRepeat;
      this->vertex[15] = 1.0f * textureRepeat;
      
      this->vertex[16] = pos3.x;
      this->vertex[17] = pos3.y;
      this->vertex[18] = pos3.z;
      this->vertex[19] = normal.x;
      this->vertex[20] = normal.y;
      this->vertex[21] = normal.z;
      this->vertex[22] = 1.0f * textureRepeat;
      this->vertex[23] = 0.0f * textureRepeat;

      this->vertex[24] = pos1.x;
      this->vertex[25] = pos1.y;
      this->vertex[26] = pos1.z;
      this->vertex[27] = normal.x;
      this->vertex[28] = normal.y;
      this->vertex[29] = normal.z;
      this->vertex[30] = 0.0f * textureRepeat;
      this->vertex[31] = 1.0f * textureRepeat;

      this->vertex[32] = pos3.x;
      this->vertex[33] = pos3.y;
      this->vertex[34] = pos3.z;
      this->vertex[35] = normal.x;
      this->vertex[36] = normal.y;
      this->vertex[37] = normal.z;
      this->vertex[38] = 1.0f * textureRepeat;
      this->vertex[39] = 0.0f * textureRepeat;

      this->vertex[40] = pos4.x;
      this->vertex[41] = pos4.y;
      this->vertex[42] = pos4.z;
      this->vertex[43] = normal.x;
      this->vertex[44] = normal.y;
      this->vertex[45] = normal.z;
      this->vertex[46] = 0.0f * textureRepeat;
      this->vertex[47] = 0.0f * textureRepeat;

      this->mesh = new Mesh(this->vertex, 8 * 6);
      this->model = glm::mat4(1.0f);
    } 

    void draw(Shader & shader, glm::mat4 * model) {

      glm::mat4 mod = this->model;
      if(model) 
        mod = mod * (*model);

      if(this->material) 
        this->material->bind(shader);

      this->mesh->draw(shader, &mod);
    }

        

    ~Plane () {
      delete this->mesh;
    }
};


#endif // PLANE_H