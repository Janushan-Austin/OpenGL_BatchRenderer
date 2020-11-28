#include "Camera.h"

Camera::Camera(glm::vec3 pos, unsigned int w, unsigned int h, float fov, float y, float p, float r) : cameraPos(pos), width(w), height(h), yaw(y), pitch(p), roll(r), fov(fov), cameraFront(0.0f, 0.0f, -1.0f), cameraRight(1.0f, 0.0f, 0.0f), cameraUp(0.0f, 1.0f, 0.0f)
{
	lastX = 0;
	lastY = 0;
}

Camera::~Camera()
{
}

glm::mat4 Camera::GenerateViewMatrix() const {
	//glm::vec3 front = glm::normalize(cameraFront);
	// //point right hand fingers in direction of first vector then curl fingers towards other vector and thumb points in the direction the cross vector points
	//glm::vec3 right = glm::normalize(glm::cross(front, cameraRotationPlaneUp));
	//glm::vec3 up = (glm::cross(right, front));


	//return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 result(1.0);

	result[0][0] = cameraRight.x;  result[1][0] = cameraRight.y;  result[2][0] = cameraRight.z;  result[3][0] = -glm::dot(cameraPos, cameraRight);
	result[0][1] = cameraUp.x;     result[1][1] = cameraUp.y;     result[2][1] = cameraUp.z;     result[3][1] = -glm::dot(cameraPos, cameraUp);
	result[0][2] = -cameraFront.x; result[1][2] = -cameraFront.y; result[2][2] = -cameraFront.z; result[3][2] = glm::dot(cameraPos, cameraFront);

	return result;
}

void Camera::WindowResizeEvent(GLFWwindow * window, int width_, int height_)
{
	width = width_;
	height = height_;
}

void Camera::MouseMovementEvent(GLFWwindow * window, double xPos, double yPos)
{
	if (firstMouseMove) {
		lastX = xPos;
		lastY = yPos;
		firstMouseMove = false;
	}

	ProcessMouseMovement((float)(xPos - lastX), (float)(yPos - lastY));

	lastX = xPos;
	lastY = yPos;
}

void Camera::MouseScrollEvent(GLFWwindow * window, double xOffset, double yOffset)
{
	ProcessMouseScroll((float)yOffset);
}
