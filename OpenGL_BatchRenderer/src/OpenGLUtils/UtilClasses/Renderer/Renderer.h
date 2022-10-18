#pragma once
#include "OpenGLUtils\UtilClasses\Models\Mesh.h"
#include "OpenGLUtils\UtilClasses\Models\Model.h"
#include "OpenGLUtils\UtilClasses\Shader\Material.h"
#include "OpenGLUtils\UtilClasses\Shader\Shader.h"
#include "OpenGLUtils\UtilClasses\Cameras\Camera.h"

#include <glm/glm.hpp>
#include <vector>

static class Renderer
{
public:

	static bool Init();
	//TODO just take a camera in to begin scene
	static void BeginScene(const glm::mat4 projectionMatrix, const Camera& camera);

	static void Draw(Mesh& mesh, glm::mat4& transform);
	static void Draw(Mesh& mesh, std::vector<glm::mat4>& transforms);
	static void Draw(std::vector<Mesh>& meshesToRender, std::vector<glm::mat4>& transforms);
	static void Draw(std::vector<Mesh>& meshesToRender, std::vector<std::vector<glm::mat4>>& transforms);

	static void EndScene();
private:

	static glm::mat4 ProjectionMatrix, ViewMatrix;
	static bool Initialized;
};

