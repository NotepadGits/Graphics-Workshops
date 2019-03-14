#version 440 core
layout (location=0) in vec3 Position; //vertex positions
layout (location=1) in vec2 texCoord;	//texture coordinates

out vec2 textureCoordinate;

void main()
{
	gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);
	textureCoordinate = vec2(texCoord.x, 1 - texCoord.y);
}