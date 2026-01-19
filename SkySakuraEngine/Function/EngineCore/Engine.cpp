#include "Engine.h"

#include <iostream>
#include <ostream>
#include <stdexcept>

Engine::Engine()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window_ = glfwCreateWindow(1024,768, "SkySakuraEngine", nullptr, nullptr);
    if (window_ == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window_);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Engine::Run()
{
    
    while(!glfwWindowShouldClose(window_))
    {
        glfwPollEvents();    
        
        process_logic_();
        process_rendering_();
        
        glfwSwapBuffers(window_);
    }
}

void Engine::ShutDown()
{
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

Engine::~Engine()
{
    glfwTerminate();
}

void Engine::process_logic_()
{
    if(glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
}


void Engine::process_rendering_()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
