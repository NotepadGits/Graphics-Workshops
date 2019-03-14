#version 440 core
out vec4 vertColour;
in vec3 Colour;			
in vec2 textureCoordinate;
in vec3 lightColour;
in float ambientIntensity;

uniform sampler2D aTex;		

void main()
{
	vec4 texColour = texture(aTex, textureCoordinate);
	vertColour = vec4(  ambientIntensity * lightColour.x * texColour.x, 
						ambientIntensity * lightColour.y * texColour.y,      	 		
						ambientIntensity * lightColour.z * texColour.z, 
						texColour.a);
}