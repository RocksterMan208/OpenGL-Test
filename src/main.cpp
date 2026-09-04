#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "error.hpp"

#include <iostream>
#include <cmath>
#include <stb/stb_image.h>

#include"shaderclass.h"
#include"vao.h"
#include"ebo.h"
#include"vbo.h"
#include"inputs.hpp"
#include"texture.h"

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
    // Positions --------- Colors in normalized RGB ------ Tex Coords
	{
		-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,       0.0f, 1.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,       1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,       1.0f, 0.0f,
	};

    GLint indices[] = 
    {
        0,2,1,
        0,3,2
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
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture

    Texture manBox("./resources/images/test.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    manBox.texUnit(shaderProgram, "tex0", 0);
    
    

    

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
        manBox.Bind();
        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Swap the back buffer to the front buffer (rendering a new frame.)
        glfwSwapBuffers(window);
        // Process all events which include keyboard presses and mouse inputs
        glfwPollEvents();
    }


    // Clearing up the code as it finishes.
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    manBox.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}