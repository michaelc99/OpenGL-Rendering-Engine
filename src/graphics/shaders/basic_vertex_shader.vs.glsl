#version 430 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 transform;
uniform mat4 projectionMatrix;

out vec3 myColor;
out vec2 myTexCoord;

void main()
{
	vec3 a = inNormal;
	myTexCoord = inTexCoord;
    gl_Position = projectionMatrix * transform * vec4(inVertex.x, inVertex.y, inVertex.z, 1.0f);
	myColor = vec3(1.0f, 0.0f, 0.0f);
}