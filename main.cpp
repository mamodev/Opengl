#include <iostream>
#include "src/reder_framework.hpp"
#define MOTION_BLUR_FRAMES 1

int main()
{   
    render_framework_init();

    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    Shader simpleDepthShader("shaders/depth.vs", "shaders/depth.fs");
    Shader skyboxShader("shaders/cubemap.vs", "shaders/cubemap.fs");

    Texture stoneTileTexture("assets/stone_tiles.png");
    Texture woodTexture("assets/wood.png");
    Texture concreteTexture("assets/ConcretePoured.png");
    Texture metalTexture("assets/MetalSteel.png");
    Texture blueTexture("assets/blue.png");

    SkyBox skyBox ("assets/skybox", &skyboxShader, glm::vec3(-1, 2, -1), glm::vec3(0.3f), glm::vec3(0.5f));

    Material stone (glm::vec3(2), glm::vec3(1), 32.0f, 1.0f, 0.0f, &stoneTileTexture);
    Material wood (glm::vec3(2), glm::vec3(0), 1.0f, 1.0f, 0.0f, &woodTexture);
    Material concrete (glm::vec3(2), glm::vec3(0), 1.0f, 1.0f, 0.0f, &concreteTexture);
    Material metal (glm::vec3(2), glm::vec3(0), 30.0f, 1.0f, 1.0f, &metalTexture);
    Material blue (glm::vec3(2), glm::vec3(0), 1.0f, 0.2f, 0.0f, &blueTexture);

    Material * materials[] = {&stone, &wood, &concrete, &blue, &metal};
    int matIndex = 0, matLength = sizeof(materials) / sizeof(Material *), matSetFlag = false;

    Light light (glm::vec3(-2.0f, 2.0f, 2.0f), glm::vec3(0.05f), glm::vec3(0.9f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
    Light light2 (glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.05f), glm::vec3(0.9f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);

    Plane plane(glm::vec3(-30, -0.5, 30), glm::vec3(30, -0.5, 30), glm::vec3(30,-0.5, -30), glm::vec3(-30, -0.5, -30), 30.0f);
    Cube statueStand (glm::vec3(0.0f, -0.3f, 0.0), glm::vec3(0.15f, 0.30f, 0.15f));
    Object statue ("assets/models/statue/statue.obj");

    plane.setMaterial(&concrete);
    statueStand.setMaterial(&concrete);

    Group * scene = new Group();
    scene->addInstance(&plane);
    scene->addInstance(&statueStand);
    scene->addInstance(&statue);
    scene->setMaterial(&wood);
    
    TimeInterpolator * interpolator = new EaseInOutCubicInterpolator();
    Animation rotateLoop (1, 0, true, interpolator);
    rotateLoop.start();


    double lightRotation = 0.0f;


    while (!window->shouldClose())
    {
        window->pollEvents();
        if(!window->focused) continue;
        camera->update(*window);

        if (!matSetFlag && rotateLoop.getProgress() > 0.5) {
          matSetFlag = true;  
          matIndex = (matIndex + 1) % matLength;
        }

        if (matSetFlag && rotateLoop.getProgress() < 0.5) {
          matSetFlag = false;
        }

        lightRotation += window->deltaTime * 0.5f;

        light.setPosition(glm::vec3(2.0f * cos(lightRotation), 2.0f, 2.0f * sin(lightRotation)));
        light2.setPosition(glm::vec3(2.0f * cos(lightRotation + 3.14f), 2.0f, 2.0f * sin(lightRotation + 3.14f)));
        // light2.setPosition(glm::vec3(2.0f * cos(-lightRotation), 2.0f, 2.0f * sin(-lightRotation)));


        rotateLoop.update(window->deltaTime);
        statue.setModel(glm::rotate(glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::radians(360.0f * rotateLoop.getProgress()), glm::vec3(0.0f, 0.0f, 1.0f)));
        statue.setMaterial(materials[matIndex]);

        light.renderShadow(simpleDepthShader, *scene);
        light2.renderShadow(simpleDepthShader, *scene);

        window->resetViewport();
        skyBox.renderSkyBox(window->projection, camera->viewMatrix);

        shader.use();
        shader.setInt("diffuseTexture", 0);
        shader.setMat4("projection", window->projection);
        skyBox.bind(shader);
        camera->activate(shader);
       
        light.bind(shader, 1);
        light2.bind(shader, 0);
        shader.setInt("lightCount", 2);
        scene->draw(shader, NULL);     
        
        window->swapBuffers();
    }

    return 0;
}