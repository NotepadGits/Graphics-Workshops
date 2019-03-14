#version 440 core

in vec2 textureCoordinate; //tex coords from vertex shader

uniform sampler2D aTex;		//uniform holding texture info from main programme

void main()
{
	gl_FragColor = texture(aTex, textureCoordinate);	
}