#version 440 core
layout (location = 0) in vec3 Position; //vertex positions
layout (location=1) in vec3 vertexColour; //vertex colours
layout (location=2) in vec2 texCoord;	//tex coords
layout (location=3) in vec3 normal;	//normals

out vec2 textureCoordinate;
out vec3 lightColour;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 lightCol;


void main()
{
	
	gl_Position = uProjection * uView * uModel * vec4(Position.x, Position.y, Position.z, 1.0); 					
	textureCoordinate = vec2(texCoord.x, 1 - texCoord.y);
	
	lightColour = lightCol;

}