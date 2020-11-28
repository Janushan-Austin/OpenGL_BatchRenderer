#version 330 core

struct MaterialProperties{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct LightProperties{
	vec3 position;

	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;
};

in vec3 fNormal;
in vec3 fPos;

uniform MaterialProperties material;
uniform LightProperties light;

uniform vec3 viewPos  = vec3(0.0);

out vec4 FragColor;

void main(){
	//calculate ambient lighting
	vec3 ambientLight = light.ambientStrength * material.ambient;

	//calculate diffuse lighting
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(light.position - fPos);

	float lightToNormalTheta = max(dot(lightDir, normal), 0.0);

	vec3 diffuseLight = (material.diffuse * lightToNormalTheta) * light.diffuseStrength;

	//calculate specular lighting
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float specPow = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specularLight = specPow * material.specular * light.specularStrength;

	vec3 result = ambientLight + diffuseLight + specularLight;

	FragColor = vec4(result, 1.0);
}