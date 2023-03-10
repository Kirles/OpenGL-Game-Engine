#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

#include <iostream>
#include <chrono>

/*For NVIDIA*/
extern "C" { _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }

/*For AMD*/
//extern "C" { _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; }

glm::ivec2 g_windowSize(640, 480);

bool isEagle = false;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        isEagle = !isEagle;
    }
}

int main(int argc, char** argv) {

    if (!glfwInit()) {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "TankBattle", nullptr, nullptr);
    if (!pWindow) {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
        std::cout << "Can`t load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Rendered: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0.5, 1, 0, 1);

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can`t create shader program: DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram) {
            std::cerr << "Can`t create shader program: SpriteShader" << std::endl;
            return -1;
        }

        std::vector<std::string> subTexturesNames = {
            "block",
            "topBlock",
            "bottomBlock",
            "leftBlock",
            "rightBlock",
            "topLeftBlock",
            "topRightBlock",
            "bottomLeftBlock",

            "bottomRightBlock",
            "beton",
            "topBeton",
            "bottomBeton",
            "leftBeton",
            "rightBeton",
            "topLeftBeton",
            "topRightBeton",

            "bottomLeftBeton",
            "bottomRightBeton",
            "water1",
            "water2",
            "water3",
            "trees",
            "ice",
            "wall",

            "eagle",
            "deadEagle",
            "nothing",
            "respawn1",
            "respawn2",
            "respawn3",
            "respawn4"

        };

        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTexturesNames), 16, 16);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
        pSprite->setPosition(glm::vec2(275, 100));

        auto pAnimatedSprite = resourceManager.loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "water1");
        pAnimatedSprite->setPosition(glm::vec2(275, 300));
        
        std::vector<std::pair<std::string, uint64_t>> waterState;
        waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1000000000));
        waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1000000000));
        waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1000000000));

        std::vector<std::pair<std::string, uint64_t>> eagleState;
        eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
        eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

        pAnimatedSprite->insertState("waterState", std::move(waterState));
        pAnimatedSprite->insertState("eagleState", std::move(eagleState));

        pAnimatedSprite->setState("waterState");

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(pWindow)) {

            if (isEagle == true) {
                pAnimatedSprite->setState("eagleState");
            }
            else {
                pAnimatedSprite->setState("waterState");
            }

            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            pAnimatedSprite->update(duration);

            glClear(GL_COLOR_BUFFER_BIT);

            pSprite->render();

            pAnimatedSprite->render();
            
            glfwSwapBuffers(pWindow);

            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}