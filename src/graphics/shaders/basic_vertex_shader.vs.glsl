#version 430 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 myColor;

void main()
{
	vec3 a = inColor;
	vec2 b = inTexCoord;
	//mat4 a = projectionMatrix * transform;
    gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0f);
	myColor = vec3(1.0f, 0.0f, 0.0f);
}