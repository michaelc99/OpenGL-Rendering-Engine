#version 430 core

uniform vec4 colorFromApplication;

in vec4 myColor;

out vec4 FragColor;

/*vec4 CalcColor(in vec3 pos) {
	pos = vec3(abs(pos.x), abs(pos.y), abs(pos.z));
	float mag = max(pos.z, max(pos.x, pos.y));
	return vec4(pos.x / mag, pos.y / mag, pos.z / mag, 1.0f);
}*/

void main()
{
    FragColor = myColor;//(myColor + colorFromApplication) / 2;
}