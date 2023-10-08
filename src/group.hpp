#ifndef GROUP_H
#define GROUP_H

#include "mesh.hpp"

class Group : public Drawable {
  public:
    glm::vec3 position;
    glm::vec3 rotation;

    std::vector<Drawable *> children;

    Group () {
      this->children = std::vector<Drawable *>();
      this->model = glm::mat4(1.0f);
      this->position = glm::vec3(0.0f);
      this->rotation = glm::vec3(0.0f);
      updateModelMatrix();
    }

    void setPosition (glm::vec3 pos) {
      this->position = pos;
      updateModelMatrix();
    }

    void setRotation (glm::vec3 rot) {
      this->rotation = rot;
      updateModelMatrix();
    }

    void addInstance (Drawable * instance) {
      this->children.push_back(instance);
    }

    void removeInstance (Drawable * instance) {
      for (int i = 0; i < this->children.size(); i++) {
        if (this->children[i] == instance) {
          this->children.erase(this->children.begin() + i);
          break;
        }
      }
    }

    void draw (Shader & shader, glm::mat4 * model) {

      for (int i = 0; i < this->children.size(); i++) {
        if(this->material) 
          this->material->bind(shader);

        if(model) 
          this->model = this->model * (*model);
        this->children[i]->draw(shader, &this->model);
      }
    }

  private: 
    inline void updateModelMatrix() {
      this->model = glm::mat4(1.0f);

      this->model = glm::rotate(this->model, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
      this->model = glm::rotate(this->model, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      this->model = glm::rotate(this->model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

      this->model = glm::translate(this->model, this->position);
    }

};


#endif // GROUP_H
