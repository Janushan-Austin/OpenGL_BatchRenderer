#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class MovementDirection {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ROLL_LEFT,
	ROLL_RIGHT,
};

const float MouseSensitivity = 0.1f;
const float CameraSpeed = 2.5f;

class Camera
{
public:
	

	Camera(glm::vec3 pos = glm::vec3(0.0f), unsigned int width = 800, unsigned int height =600, float fov = 90.0f, float y = 0.0f, float p = 0.0f, float r = 0.0f);
	virtual ~Camera();

	virtual glm::mat4 GenerateViewMatrix() const ;


	inline const glm::vec3& Position() const { return cameraPos; }
	inline const glm::vec3& Front() const { return cameraFront; }
	inline const glm::vec3& Right() const { return cameraRight; }
	inline const glm::vec3& Up() const { return cameraUp; }
	inline const float& FOV() const { return fov; }
	inline const unsigned int& Height() const  { return height; }
	inline const unsigned int& Width() const  { return width; }
	inline void SetDimmensions(unsigned int width_, unsigned int height_) { width = width_; height = height_; }

	virtual void ProcessMouseMovement(float xOffset, float yOffset) =0;
	virtual void ProcessKeyboardInput(MovementDirection direction, float deltaTime) = 0;
	virtual void ProcessMouseScroll(float yOffset) =0;
	void WindowResizeEvent(GLFWwindow* window, int width, int height);
	void MouseMovementEvent(GLFWwindow* window, double xPos, double yPos);
	void MouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset);

protected:
	glm::vec3 cameraPos, cameraFront, cameraRight, cameraUp;

	unsigned int width, height;

	float yaw, pitch, roll;

	float fov;

	//used for mouse movement callback for the activeCamera
	bool firstMouseMove;
	double lastX, lastY;
};

