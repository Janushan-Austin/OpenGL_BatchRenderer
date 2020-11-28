#version 330 core

out vec4 FragColor;

uniform vec3 objectColor, lightColor;

void main(){
	FragColor = vec4(objectColor * lightColor, 1.0);
}