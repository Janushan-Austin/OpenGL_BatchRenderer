#pragma once
#include <string>
#include "OpenGLUtils/Includes/OpenGLIncludeHeaders.h"

struct ShaderProgramSource {
	std::string vertexShader;
	std::string fragmentShader;
};

enum class shaderType {
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
};

class Shader
{
public:
	Shader(const std::string& shaderFilePath);
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	Shader(const Shader& shaderCopy);
	~Shader();

	void Bind();
	void UnBind();

	void SetInt1(const std::string& name, int v0);
	void SetInt2(const std::string& name, int v0, int v1);
	void SetInt3(const std::string& name, int v0, int v1, int v2);
	void SetInt4(const std::string& name, int v0, int v1, int v2, int v3);

	void SetFloat1(const std::string& name, float v0);
	void SetFloat2(const std::string& name, float v0, float v1);
	void SetFloat3(const std::string& name, float v0, float v1, float v2);
	void SetFloat4(const std::string& name, float v0, float v1, float v2, float v3);

	void SetVec3(const std::string& name, glm::vec3);
	void SetVec3(const std::string& name, float v0, float v1, float v2);
	void SetMat4(const std::string& name, glm::mat4 matrix);

	int GetUniformLocation(const std::string& name);

private:
	unsigned int shaderID;

	ShaderProgramSource ParseShader(const std::string& shaderFilePath, shaderType initialType = shaderType::NONE);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(int type, const std::string& shader);
};

