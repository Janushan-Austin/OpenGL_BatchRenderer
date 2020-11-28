#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main(){
	vec4 texColor = texture(texture_diffuse1, fTexCoord);
	if(texColor.a < 0.1)
	{
		discard;
	}
	FragColor = texColor;
//	FragColor = vec4(1.0f, 0.0, 0.0, 1.0f);
}