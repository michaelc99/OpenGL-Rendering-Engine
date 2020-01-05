#version 430 core

uniform sampler2D texture0;
uniform sampler2D texture1;

in vec3 myColor;
in vec2 myTexCoord;

out vec4 FragColor;

void main()
{
	vec3 a = myColor;
	FragColor = mix(texture(texture0, myTexCoord), texture(texture1, myTexCoord), 0.5f);
	//FragColor = vec4(myColor, 1.0f);
}