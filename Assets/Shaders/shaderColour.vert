#version 440 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Colour;

out vec3 vertexColour;

void main()
{
	gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);
	vertexColour = Colour;

}