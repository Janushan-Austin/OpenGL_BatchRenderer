#include "CallBackFunctions.h"


Event<void, GLFWwindow*, int, int> framebufferResizeEvent;
Event<void, GLFWwindow*, double, double> mouseMovementEvent;
Event<void, GLFWwindow*, double, double> mouseScrollEvent;

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	framebufferResizeEvent(window, newWidth, newHeight);
}

void mouse_Movement_Callback(GLFWwindow* window, double xPos, double yPos) {
	mouseMovementEvent(window, xPos, yPos);
}

void mouse_Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset) {
	mouseScrollEvent(window, xOffset, yOffset);
}

