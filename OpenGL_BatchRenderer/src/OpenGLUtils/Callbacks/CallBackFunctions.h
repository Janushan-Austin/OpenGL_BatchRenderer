#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "OpenGLUtils/UtilClasses/Event/Event.h"

//external refernce for anyone including OpenGL callbacks so they can subscribe to their desired event
extern Event<void, GLFWwindow*, int, int> framebufferResizeEvent;
extern Event<void, GLFWwindow*, double, double> mouseMovementEvent;
extern Event<void, GLFWwindow*, double, double> mouseScrollEvent;

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight);
void mouse_Movement_Callback(GLFWwindow* window, double xPos, double yPos);
void mouse_Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset);