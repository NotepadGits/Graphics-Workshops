#version 440 core
layout (location = 0) in vec3 Pos;

void main()
{
	gl_Position = vec4(Pos.x, Pos.y, Pos.z, 0.5);

}