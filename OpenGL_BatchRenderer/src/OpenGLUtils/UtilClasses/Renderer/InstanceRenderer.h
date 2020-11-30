#pragma once
#include "OpenGLUtils\UtilClasses\Models\Mesh.h"
#include "OpenGLUtils\UtilClasses\Models\Model.h"
#include "OpenGLUtils\UtilClasses\Shader\Material.h"
#include "OpenGLUtils\UtilClasses\Shader\Shader.h"
#include "OpenGLUtils\UtilClasses\Cameras\Camera.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

class InstanceRenderer
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

	static void Flush();
	static void PutInRenderingBucket(Material* material, MaterialID materialID, Mesh* mesh, MeshID meshId, std::vector<glm::mat4>& transforms);

	struct MeshMapStruct
	{
		Mesh* mesh;
		std::vector<glm::mat4> meshTransforms;
		uint32_t instanceCount;
	};

	struct MaterialMapStruct
	{
		Material* material;
		std::unordered_map<MeshID, MeshMapStruct> meshMap;
	};

	static std::unordered_map<MaterialID, MaterialMapStruct> MaterialMap;
	static glm::mat4 ProjectionMatrix, ViewMatrix;

	static uint32_t InstancingVAO, InstancingVBO;
	static uint32_t MaxInstances;
	static bool Initialized;
};