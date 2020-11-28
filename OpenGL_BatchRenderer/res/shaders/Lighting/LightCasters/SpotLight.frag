#version 330 core

struct MaterialProperties{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct LightProperties{
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;

	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;

	float constant;
	float linear;
	float quadratic;
};

in vec3 fNormal;
in vec3 fPos;
in vec2 fTexCoord;

uniform MaterialProperties material;
uniform LightProperties light;

uniform vec3 viewPos  = vec3(0.0);

out vec4 FragColor;

void main(){
	//calculate ambient lighting
	vec3 ambientLight = light.ambientStrength * texture(material.diffuse, fTexCoord).rgb;

	//used to store results of lighting calculations
	vec3 result;
	
	//vector from fragment position to light position
	vec3 fragToLightDir = normalize(light.position - fPos);

	//calculate strength of the light
	float distance = length(light.position - fPos);
	float attenuation = 1.0 / (light.constant +  light.linear * distance + light.quadratic * distance * distance);

	//angle between light direction and fragment to light direction vectors
	float theta = dot(normalize(-light.direction), fragToLightDir);
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	//calculate diffuse lighting
	vec3 normal = normalize(fNormal);
	float lightToNormalTheta = max(dot(fragToLightDir, normal), 0.0);

	vec3 diffuseLight = (texture(material.diffuse, fTexCoord).xyz * lightToNormalTheta) * light.diffuseStrength;

	//calculate specular lighting
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-fragToLightDir, normal);
	
	float specPow = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specularLight = specPow * texture(material.specular, fTexCoord).xyz * light.specularStrength;

	result = (ambientLight + (diffuseLight + specularLight) * intensity) * attenuation;

	FragColor = vec4(result, 1.0);
}