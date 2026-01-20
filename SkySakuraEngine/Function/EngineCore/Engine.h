#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Engine
{
public:
    Engine();
    
    void Run();
    
    void ShutDown();
    
    ~Engine();
    
private:
    GLFWwindow* window_;
    unsigned int shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    
    void process_logic_();
    void process_rendering_();
    
    void initialize_shader_();
};
