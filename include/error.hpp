#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

void CheckForWindow(GLFWwindow* window) // Error checks to see if the window failed to create
{
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
    }
}