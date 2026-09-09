#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

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
#include"camera.h"
#include"mesh.h"

#define SCR_W 1920
#define SCR_H 1080

float FOVY = 45.0f;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, "LearnOpenGL", NULL, NULL);

    CheckForWindow(window);

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, SCR_W, SCR_H);

    Shader shaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    Mesh cubeMesh(CUBE);

    Texture manBox("./resources/images/test.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    manBox.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Camera camera(SCR_W, SCR_H, glm::vec3(0.0f, 0.0f, 2.0f));

    std::vector<glm::vec3> positions = genChunk(16,16,16);


    while (!glfwWindowShouldClose(window))
    {
        ProcessInputs(window);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        camera.ProcessInputs(window);

        manBox.Bind();

        for (const auto& pos : positions)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

            camera.Matrix(FOVY, 0.1f, 100.0f, shaderProgram, "camMatrix", model);
            cubeMesh.Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    manBox.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}