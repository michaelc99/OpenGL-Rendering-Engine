#version 430 core

in vec3 myColor;

out vec4 FragColor;

void main()
{
	//FragColor = mix(texture(texture0, myTexCoord), texture(texture1, vec2(1 - myTexCoord.x, myTexCoord.y)), 0.5f);
	FragColor = vec4(myColor, 1.0f);
}