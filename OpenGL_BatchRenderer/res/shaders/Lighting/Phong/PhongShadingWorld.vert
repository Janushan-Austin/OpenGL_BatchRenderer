#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

uniform mat4 model, view, projection;

out vec3 fNormal;
out vec3 fPos;

void main(){
	gl_Position = projection * view * model * vec4(vPos, 1.0);

	fPos = (model * vec4(vPos, 1.0)).xyz;

	fNormal = mat3(transpose(inverse(model))) * vNormal;
}