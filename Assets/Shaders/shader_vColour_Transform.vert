#version 440 core
layout (location = 0) in vec3 Position; //vertex positions
layout (location=1) in vec3 vColour;	//vertex colours
layout (location=2) in vec2 texCoord;	//texture coordinates

out vec3 Colour; 						//output vertex colour to fragment shader
out vec2 textureCoordinate;

uniform mat4 uTransform;				//pass transform matrix into the shader

void main()
{
	
	gl_Position = uTransform * vec4(Position, 1.0); //update position of vertex after transfrom
	Colour = vColour; 								//pass the vertex colour unchanged to the fragment shader
	textureCoordinate = vec2(texCoord.x, 1 - texCoord.y);

}