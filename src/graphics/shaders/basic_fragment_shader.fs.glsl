#version 430 core

uniform float mixVal;
uniform sampler2D texture0;
uniform sampler2D texture1;

in vec3 myColor;
in vec2 myTexCoord;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(texture0, myTexCoord), texture(texture1, vec2(1 - myTexCoord.x, myTexCoord.y)), mixVal);
}