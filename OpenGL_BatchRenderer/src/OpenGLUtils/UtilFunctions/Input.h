#pragma once
#include "OpenGLUtils/OpenGLUtils.h"

void processInput(GLFWwindow* window);
void processCameraInput(GLFWwindow* window, Camera& camera, float deltaTime);
void processCameraInput(GLFWwindow * window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float cameraSpeed = 0.5f, float deltaTime = 1.0f);