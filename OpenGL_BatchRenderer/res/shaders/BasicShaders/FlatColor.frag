#version 330 core

out vec4 FragColor;
uniform vec3 FlatColor;

void main()
{
	FragColor = vec4(FlatColor, 1.0);	
}