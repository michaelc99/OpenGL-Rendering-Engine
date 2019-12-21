#version 430 core

uniform vec4 colorFromApplication;

in vec4 myColor;

out vec4 FragColor;

void main()
{
    FragColor = (myColor + colorFromApplication) / 2;
}