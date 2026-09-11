#pragma once
#include <GLFW/glfw3.h>
#include"camera.h"
#include"chunk.h"

#include"imgui/imgui.h"
#include"imgui/backends/imgui_impl_glfw.h"
#include"imgui/backends/imgui_impl_opengl3.h"

int wasPressed = 0;
int lockedMouse = 1;
extern int chunksX, chunksY;


size_t numBlocks = 0;

void ProcessInputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

void initImGUI(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void startImGUIFrame(Camera* camera)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();

    ImGui::Begin("Voxel Engine");

    ImGui::Text("Camera Position:");
    ImGui::DragFloat("X", &camera->position.x, 1.0f, -FLT_MAX, FLT_MAX, "%.3f");
    ImGui::DragFloat("Y", &camera->position.y, 1.0f, -FLT_MAX, FLT_MAX, "%.3f");
    ImGui::DragFloat("Z", &camera->position.z, 1.0f, -FLT_MAX, FLT_MAX, "%.3f");

    ImGui::Text("Chunk Position:");
    ImGui::Text("X: %d", static_cast<int>(std::floor(camera->position.x / 16)));
    ImGui::Text("Z: %d", static_cast<int>(std::floor(camera->position.z / 16)));

    ImGui::Text("Noise Manipulation");
    ImGui::SliderFloat("Factor", &intensity, 1.0f, 0.0f);

    ImGui::Text("Chunk Manipulation: NOT IMPLEMENTED");
    ImGui::SliderInt("Chunks X:", &chunksX, 0, 60);
    ImGui::SliderInt("Chunks Y:", &chunksY, 0, 60);

    ImGui::End();
}

void renderImGUI()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void stopImGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void cursorToggle(GLFWwindow* window, int* mouseVar)
{
    int isPressed = glfwGetKey(window, GLFW_KEY_X);

        if (isPressed && !wasPressed)
        {
            if (!lockedMouse)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                *mouseVar = 1;
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                *mouseVar = 0;
            }
            lockedMouse = !lockedMouse;
        }

        wasPressed = isPressed;
}