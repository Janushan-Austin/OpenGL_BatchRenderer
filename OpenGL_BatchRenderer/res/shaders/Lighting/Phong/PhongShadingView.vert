#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

uniform mat4 model, view, projection;

uniform vec3 worldLightPos = vec3(0.0, 0.0, 0.0);

out vec3 fNormal;
out vec3 fPos;
out vec3 lightPos;

void main(){
	gl_Position = projection * view * model * vec4(vPos, 1.0);

	fPos = (view * model * vec4(vPos, 1.0)).xyz;

	fNormal = mat3(transpose(inverse(view * model))) * vNormal;

	lightPos = (view * vec4(worldLightPos, 1.0)).xyz;
}