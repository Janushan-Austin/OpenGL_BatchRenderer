#version 330 core

struct MaterialProperties{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;

	float shininess;
};

struct SpotLightProperties{
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

struct PointLightProperties{
	vec3 position;

	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;

	float constant;
	float linear;
	float quadratic;
};

struct DirectionLightProperties{
	vec3 direction;

	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;
};

in vec3 fNormal;
in vec3 fPos;
in vec2 fTexCoord;

#define NR_POINTLIGHTS 4

uniform int numberPointLightsPassed = 0;

uniform MaterialProperties material;
uniform DirectionLightProperties directionalLight;
uniform PointLightProperties pointLights[NR_POINTLIGHTS];
uniform SpotLightProperties spotLight;

uniform vec3 viewPos  = vec3(0.0);

out vec4 FragColor;

vec3 CalculateDirectionalLighting(DirectionLightProperties dirLight, vec3 normal, vec3 viewDir);
vec3 CalculatePointLighting(PointLightProperties pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLighting(SpotLightProperties spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){

	//used to store results of lighting calculations
	vec3 lightingResult = vec3(0.0);
	vec3 normal = normalize(fNormal);
	vec3 viewDirection = normalize(viewPos - fPos);

	lightingResult += CalculateDirectionalLighting(directionalLight, normal, viewDirection);

	for(int i=0; i < numberPointLightsPassed; i++)
	{
		lightingResult += CalculatePointLighting(pointLights[i], normal, fPos, viewDirection);
	}

	//lightingResult += CalculateSpotLighting(spotLight, normal, fPos, viewDirection);

	FragColor = vec4(lightingResult, 1.0);
}


vec3 CalculateDirectionalLighting(DirectionLightProperties dirLight, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-dirLight.direction);
	float lightToNormalTheta = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);	
	float specPow = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//calculate actual lighting values
	vec3 ambientLight = dirLight.ambientStrength * texture(material.texture_diffuse1, fTexCoord).rgb;
	vec3 diffuseLight = dirLight.diffuseStrength * lightToNormalTheta * texture(material.texture_diffuse1, fTexCoord).xyz ;
	//TODO figure out why specPow is incorrect below
	vec3 specularLight = dirLight.specularStrength * specPow * texture(material.texture_specular1, fTexCoord).xyz ;

	return  ambientLight + diffuseLight + specularLight;
}

vec3 CalculatePointLighting(PointLightProperties pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(pointLight.position - fragPos);
	float lightToNormalTheta = max(dot(lightDir, normal), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);	
	float specPow = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//calculate intensity of the light
	float distance = length(pointLight.position - fragPos);
	float attenuation = 1.0 / (pointLight.constant +  pointLight.linear * distance + pointLight.quadratic * distance * distance);

	//calculate actual lighting values
	vec3 ambientLight = pointLight.ambientStrength * texture(material.texture_diffuse1, fTexCoord).rgb;
	vec3 diffuseLight = (texture(material.texture_diffuse1, fTexCoord).xyz * lightToNormalTheta) * pointLight.diffuseStrength;
	vec3 specularLight = specPow * texture(material.texture_specular1, fTexCoord).xyz * pointLight.specularStrength;

	return  (ambientLight + diffuseLight + specularLight) * attenuation;
}

vec3 CalculateSpotLighting(SpotLightProperties spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(spotLight.position - fragPos);
	float lightToNormalTheta = max(dot(lightDir, normal), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);	
	float specPow = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//calculate intensity of the light
	float distance = length(spotLight.position - fragPos);
	float attenuation = 1.0 / (spotLight.constant +  spotLight.linear * distance + spotLight.quadratic * distance * distance);

	//angle between light direction and fragment to light direction vectors
	float theta = dot(normalize(-spotLight.direction), lightDir);
	float epsilon = spotLight.cutoff - spotLight.outerCutoff;
	float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0, 1.0);

	//calculate actual lighting values
	vec3 ambientLight = spotLight.ambientStrength * texture(material.texture_diffuse1, fTexCoord).rgb;
	vec3 diffuseLight = texture(material.texture_diffuse1, fTexCoord).xyz * lightToNormalTheta * spotLight.diffuseStrength;
	vec3 specularLight = specPow * texture(material.texture_specular1, fTexCoord).xyz * spotLight.specularStrength;

	return  (ambientLight + diffuseLight + specularLight) * attenuation * intensity;
}
