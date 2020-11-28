#pragma once
#include <string>
#include <vector>
#include "OpenGLUtils/Includes/OpenGLIncludeHeaders.h"

class Shader;
class Material;

typedef uint32_t MeshID;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	//Texture Coordinate
	glm::vec2 TexCoord;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

enum class TextureType {
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT
};

struct Texture {
	unsigned int id;
	TextureType type;
	std::string path;
};

class Mesh {
public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;

	void Draw( Shader& shader, const std::string& uniformName = "");
	void Draw();

	void UploadUniformsToMaterial() { UploadTexturesToMaterial(); }
	void UploadTexturesToMaterial(const std::string& uniformName);
	void UploadTexturesToMaterial();

	unsigned int GetVAO() const { return VAO; }
	unsigned int GetVBO() const { return VBO; }
	unsigned int GetEBO() const { return EBO; }
	unsigned int GetIndicesCount() const { return Indices.size(); }

	MeshID GetMeshID() const { return meshID; }

	Material* GetMaterial() const { return MeshMaterial; };
	void SetMaterial(Material& material);

	void SetUniformContainer(const std::string& uniformContainerName);

private:
	MeshID meshID;
	static MeshID NextMeshID;


	Material* MeshMaterial;
	std::string UniformContainerName;

	unsigned int VAO, VBO, EBO;

	void SetupMesh();

};