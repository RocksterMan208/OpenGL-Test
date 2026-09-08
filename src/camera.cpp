#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 Position)
{
    Camera::width = width;
    Camera::height = height;
    position = Position;
}

void Camera::Matrix(float FOVyDeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(position, position + direction, upDir);
    projection = glm::perspective(static_cast<float>(glm::radians(FOVyDeg)), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));

}

void Camera::ProcessInputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += cameraSpeed * direction;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position += cameraSpeed * -glm::normalize(glm::cross(direction, upDir));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position += cameraSpeed * -direction;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += cameraSpeed * glm::normalize(glm::cross(direction, upDir));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        position += cameraSpeed * upDir;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        position += cameraSpeed * -upDir;
    }
}