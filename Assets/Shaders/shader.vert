#version 440 core
layout (location = 0) in vec3 Position; //vertex positions
layout (location=1) in vec3 vColour;	//vertex colours
layout (location=2) in vec2 texCoord;	//texture coordinates

out vec3 Colour; 						//output vertex colour to fragment shader
out vec2 textureCoordinate;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;

void main()
{
	
		vec3 offset;
	offset.x = sin(Position.y * 3 + uTime * 0.005)*0.05;


	gl_Position = uProjection*uView*uModel*vec4(Position + offset, 1.0); //update position of vertex after transfrom
	Colour = vColour; 								//pass the vertex colour unchanged to the fragment shader




	textureCoordinate = vec2(texCoord.x, 1 - texCoord.y);

}