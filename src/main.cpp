#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "error.hpp"

#include <iostream>
#include <cmath>

#include"shaderclass.h"
#include"vao.h"
#include"ebo.h"
#include"vbo.h"
#include"inputs.hpp"

#define SCR_W 800
#define SCR_H 600

int main()
{
    // Start GLFW
    glfwInit();
    
    // Stating to GLFW what version is being used.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // Vertices coordinates
	GLfloat vertices[] =
    // Positions ---------------------------------------- Colors in normalized RGB
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,        1.0f, 0.0f, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,         0.0f, 1.0f, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,      0.0f, 0.0f, 1.0f,// Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,     1.0f, 0.0f, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,      0.0f, 1.0f, 0.0f,   // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,         0.0f, 0.0f, 1.0f,// Inner down
	};

    GLint indices[] = 
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };

    // Creating then verifying that a window has been created.
    GLFWwindow* window = glfwCreateWindow(SCR_W,SCR_H, "LearnOpenGL", NULL, NULL);

    //Error checks to see if the window will fail to open or not
    CheckForWindow(window);

    // Introduce the current window to the current context
    glfwMakeContextCurrent(window);

    // Initialise and verify that GLAD has loaded correctly and is working
    gladLoadGL();

    // Specify the range in which GLFW operates on the screen.
    glViewport(0,0, SCR_W, SCR_H);

    // Loading the shaders from a file instead of having them be inside a const char* variable
    Shader shaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    VAO VAO1;
    VAO1.Bind();

    // Creating the arrays using helper functions
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));


    // Link attrib inputs the layout and positions to the shaders which then can in unison create the image.
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        ProcessInputs(window);
        // Specify which color the background,  also known as the void is.
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Drawing Functions
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        
        // Swap the back buffer to the front buffer (rendering a new frame.)
        glfwSwapBuffers(window);
        // Process all events which include keyboard presses and mouse inputs
        glfwPollEvents();
    }


    // Clearing up the code as it finishes.
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}