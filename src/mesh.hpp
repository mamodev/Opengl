#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>  
#include <glm/glm.hpp>
#include <assert.h>

#include "./shader.hpp"
#include "./material.hpp"

// Vertex struct
// [ x, y, z, nx, ny, nz, u, v]
#define VERTEX_SIZE 8

class Drawable {
  public:
    virtual void draw(Shader& shader, glm::mat4 * model) = 0;

    Drawable () {
      this->material = NULL;
      this->model = glm::mat4(1.0f);
    }

    void setModel (glm::mat4 model) {
      this->model = model;
    }

    void setMaterial (Material * material) {
      this->material = material;
    }
  protected:  
    Material * material;
    glm::mat4 model;
};

void setVec3ToArr (GLfloat * arr, glm::vec3 vec, int index) {
  arr[index] = vec.x;
  arr[index + 1] = vec.y;
  arr[index + 2] = vec.z;
}


class Mesh : public Drawable {
  public: 
    GLuint VBO, VAO;
    GLfloat * vertex;

    int vertexSize, indicesSize;

    Mesh (GLfloat * vertex, int vertexSize) {
      this->vertex = vertex;
      this->vertexSize = vertexSize;

      glGenVertexArrays(1, &this->VAO);
      glGenBuffers(1, &this->VBO);

      assert(this->VAO != 0 && this->VBO != 0);

      glBindVertexArray(this->VAO);

      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glBufferData(GL_ARRAY_BUFFER, this->vertexSize * sizeof(GLfloat), this->vertex, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glBindVertexArray(0);
    }

    ~Mesh () {
      glDeleteVertexArrays(1, &this->VAO);
      glDeleteBuffers(1, &this->VBO);
    }

    void draw(Shader& shader, glm::mat4 * model) {

      glm::mat4 mod = this->model;
      if(model) 
        mod = mod * (*model);

      shader.setMat4("model", mod);

      if(this->material) 
        this->material->bind(shader);

      glBindVertexArray(this->VAO);
      glDrawArrays(GL_TRIANGLES, 0, this->vertexSize / VERTEX_SIZE);
      glBindVertexArray(0);
    }

};

// class Mesh : public Drawable {
//   public: 
//     GLuint VBO, VAO;
//     GLfloat * vertex;

//     int vertexSize;

//     Mesh (GLfloat * vert, int vertexSize) {

//       // Take vertex data in form [ x, y, z, nx, ny, nz, u, v] 
//       // and add tangent and bitangent vectors to it
//       int nVertex = vertexSize / 8;
//       int nTriangles = nVertex / 3;

//       this->vertexSize = nVertex * (8 + 6);
//       this->vertex = (GLfloat *) malloc(sizeof(GLfloat) * this->vertexSize);

//       // Copy old vertex data to new vertex array 
//       for (int v = 0; v < nVertex; ++v) {
//         for (int i = 0; i < 8; i++) {
//           assert(v * 14 + i < this->vertexSize);
//           assert(v * 8 + i < vertexSize);
//           this->vertex[v * 14 + i] = vert[v * 8 + i];
//         }
//       }

//       for (int t = 0; t < nTriangles; t++) {
//         glm::vec3 tan (0.0f), bitan (0.0f);

//         int v1 = t * VERTEX_SIZE;
//         int v2 = v1 + VERTEX_SIZE;
//         int v3 = v2 + VERTEX_SIZE;

//         glm::vec3 pos1 (vertex[v1], vertex[v1 + 1], vertex[v1 + 2]);
//         glm::vec3 pos2 (vertex[v2], vertex[v2 + 1], vertex[v2 + 2]);
//         glm::vec3 pos3 (vertex[v3], vertex[v3 + 1], vertex[v3 + 2]);

//         glm::vec2 uv1 (vertex[v1 + 6], vertex[v1 + 7]);
//         glm::vec2 uv2 (vertex[v2 + 6], vertex[v2 + 7]);
//         glm::vec2 uv3 (vertex[v3 + 6], vertex[v3 + 7]);

//         glm::vec3 edge1 = pos2 - pos1;
//         glm::vec3 edge2 = pos3 - pos1;

//         glm::vec2 deltaUV1 = uv2 - uv1;
//         glm::vec2 deltaUV2 = uv3 - uv1;

//         float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

//         glm::vec3 tangent;
//         tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
//         tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
//         tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

//         glm::vec3 bitangent;
//         bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
//         bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
//         bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

//         tan += tangent;
//         bitan += bitangent;

//         setVec3ToArr(this->vertex, tan, v1 + 8);
//         setVec3ToArr(this->vertex, tan, v2 + 8);
//         setVec3ToArr(this->vertex, tan, v3 + 8);

//         setVec3ToArr(this->vertex, bitan, v1 + 11);
//         setVec3ToArr(this->vertex, bitan, v2 + 11);
//         setVec3ToArr(this->vertex, bitan, v3 + 11);
//       }

//       glGenVertexArrays(1, &this->VAO);
//       glGenBuffers(1, &this->VBO);

//       assert(this->VAO != 0 && this->VBO != 0);

//       glBindVertexArray(this->VAO);

//       glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//       glBufferData(GL_ARRAY_BUFFER, this->vertexSize * sizeof(GLfloat), this->vertex, GL_STATIC_DRAW);

//       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)0);
//       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//       glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//       glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
//       glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));

//       glEnableVertexAttribArray(0);
//       glEnableVertexAttribArray(1);
//       glEnableVertexAttribArray(2);
//       glEnableVertexAttribArray(3); 
//       glEnableVertexAttribArray(4);

//       glBindBuffer(GL_ARRAY_BUFFER, 0);

//       glBindVertexArray(0);
//     }

//     ~Mesh () {
//       glDeleteVertexArrays(1, &this->VAO);
//       glDeleteBuffers(1, &this->VBO);
//     }

//     void setModel (glm::mat4 model) {
//       this->model = model;
//     }

//     void draw(Shader& shader, glm::mat4 * model) {

//       glm::mat4 mod = this->model;
//       if(model) 
//         mod = mod * (*model);

//       shader.setMat4("model", mod);

//       if(this->material) 
//         this->material->bind(shader);

//       glBindVertexArray(this->VAO);
//       glDrawArrays(GL_TRIANGLES, 0, this->vertexSize / VERTEX_SIZE);
//       glBindVertexArray(0);
//     }

// };


#endif  // MESH_H