#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 myColor;
out vec2 myTexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	myColor = vec3(inColor);
	myTexCoord = inTexCoord;
}