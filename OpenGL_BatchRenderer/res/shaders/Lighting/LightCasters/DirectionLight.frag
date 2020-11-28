#version 330 core

struct MaterialProperties{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct LightProperties{
	vec3 direction;

	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;
};

in vec3 fNormal;
in vec3 fPos;
in vec2 fTexCoord;

uniform MaterialProperties material;
uniform LightProperties light;

uniform vec3 viewPos;

out vec4 FragColor;

void main(){
	//calculate ambient lighting
	vec3 ambientLight = light.ambientStrength * texture(material.diffuse, fTexCoord).xyz;

	//calculate diffuse lighting
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(-light.direction);

	float lightToNormalTheta = max(dot(lightDir, normal), 0.0);

	vec3 diffuseLight = texture(material.diffuse, fTexCoord).xyz * lightToNormalTheta * light.diffuseStrength;

	//calculate specular lighting
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	
	float specPow = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//specPow = 0;

	vec3 specularLight = light.specularStrength * specPow * texture(material.specular, fTexCoord).xyz;

	vec3 result = ambientLight + diffuseLight + specularLight;

	FragColor = vec4(result, 1.0);
}