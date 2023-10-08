#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include <stdio.h>  
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "./tiny_obj_loader.hpp"

#include <GL/glew.h>
#include "./mesh.hpp"
#include "./shader.hpp"

typedef struct  {
  Mesh * mesh;
  GLfloat * vertices;
  int verticesSize;
} ShapeData;

class Object : public Drawable {
  public: 
    ShapeData * shapes;
    int shapesSize;

    Object (const char * path) {
      this->model = glm::mat4(1.0f);

	    tinyobj::ObjReader reader;
      tinyobj::ObjReaderConfig reader_config = tinyobj::ObjReaderConfig();
      reader_config.triangulate = true;

      if (!reader.ParseFromFile(path, reader_config)) {
        if (!reader.Error().empty()) {
          fprintf(stderr, "TinyObjReader: %s", reader.Error().c_str());
        }
        exit(1);
      }

      if (!reader.Warning().empty()) {
        fprintf(stderr, "TinyObjReader: %s", reader.Warning().c_str());
      }

      auto attrib = reader.GetAttrib();
      auto shapes = reader.GetShapes();
      auto materials = reader.GetMaterials();
      
      if(shapes.size() == 0) {
        fprintf(stderr, "No shapes found in object file %s\n", path);
        exit(1);
      }

      if(attrib.normals.size() == 0) {
        fprintf(stderr, "No normals found in object file %s\n", path);
        exit(1);
      }

      // if(attrib.texcoords.size() == 0) {
      //   fprintf(stderr, "No texture coordinates found in object file %s\n", path);
      //   exit(1);
      // }

      this->shapes = new ShapeData[shapes.size()];
      this->shapesSize = shapes.size();

      int shapeIndex = 0;
      for (const auto& shape : shapes) {
        this->shapes[shapeIndex].verticesSize = shape.mesh.indices.size() * 8;
        this->shapes[shapeIndex].vertices = new GLfloat[this->shapes[shapeIndex].verticesSize];

        int pos = 0;
        for(const auto& index : shape.mesh.indices) {
          assert(index.vertex_index >= 0);
          assert(index.normal_index >= 0);
          // assert(index.texcoord_index >= 0);
          
          this->shapes[shapeIndex].vertices[pos] = attrib.vertices[3 * index.vertex_index + 0];
          this->shapes[shapeIndex].vertices[pos + 1] = attrib.vertices[3 * index.vertex_index + 1];
          this->shapes[shapeIndex].vertices[pos + 2] = attrib.vertices[3 * index.vertex_index + 2];

          this->shapes[shapeIndex].vertices[pos + 3] = attrib.normals[3 * index.normal_index + 0];
          this->shapes[shapeIndex].vertices[pos + 4] = attrib.normals[3 * index.normal_index + 1];
          this->shapes[shapeIndex].vertices[pos + 5] = attrib.normals[3 * index.normal_index + 2];

          if(attrib.texcoords.size() != 0) {
            this->shapes[shapeIndex].vertices[pos + 6] = attrib.texcoords[2 * index.texcoord_index + 0];
            this->shapes[shapeIndex].vertices[pos + 7] = attrib.texcoords[2 * index.texcoord_index + 1];
          }     

          pos = pos + 8;
        }

  
        this->shapes[shapeIndex].mesh = new Mesh(this->shapes[shapeIndex].vertices, this->shapes[shapeIndex].verticesSize);

        shapeIndex++;
      }

    }

    ~Object () {
      for (int i = 0; i < this->shapesSize; i++) {
        delete this->shapes[i].mesh;
        delete this->shapes[i].vertices;
      }

      delete this->shapes;
    }

    void draw (Shader & shader, glm::mat4 * model) {
      if(this->material) 
        this->material->bind(shader);
        
      for (int i = 0; i < this->shapesSize; i++) {
        if(model) 
          this->model = this->model * (*model);
        this->shapes[i].mesh->draw(shader, &this->model);
      }
    }
};

#endif // OBJECT_LOADER_H
