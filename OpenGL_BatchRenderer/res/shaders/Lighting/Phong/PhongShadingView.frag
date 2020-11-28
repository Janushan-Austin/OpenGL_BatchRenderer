#version 330 core

in vec3 fNormal;
in vec3 fPos;
in vec3 lightPos;

uniform vec3 lightColor = vec3(1.0);
uniform float ambientStrength = 0.1;

uniform vec3 objectColor = vec3(1.0);

uniform float specularStrength = 0.5;

out vec4 FragColor;

void main(){
	//calculate ambient lighting
	vec3 ambientLight = lightColor * ambientStrength;

	//calculate diffuse lighting
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fPos);

	float lightToNormalTheta = max(dot(lightDir, normal), 0.0);

	vec3 diffuseLight = lightToNormalTheta * lightColor;

	//calculate specular lighting
	vec3 viewDir = normalize(-fPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float specPow = pow(max(dot(viewDir, reflectDir), 0.0), 64);

	vec3 specularLight = specPow * specularStrength * lightColor;

	vec3 result = (ambientLight + diffuseLight + specularLight) * objectColor;

	FragColor = vec4(result, 1.0);
}