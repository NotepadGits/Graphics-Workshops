#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec3 Colour;			//input the Colour values from Vertex shader
in vec2 textureCoordinate; //tex coords from vertex shader

uniform sampler2D aTex;		//uniform holding texture info from main programme

void main()
{
	//vertColour = vec4(Colour, 1.0f);
	//vertColour = texture(aTex, textureCoordinate);
	gl_FragColor = texture(aTex, textureCoordinate);
	
	
}