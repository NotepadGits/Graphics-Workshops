#version 440 core
layout (location = 0) in vec3 Position; //vertex positions
layout (location=1) in vec3 vColour;	//vertex colours

out vec3 Colour; 						//output vertex colour to fragment shader

uniform float uTime;					//uniform variable modified in main code


void main()
{
	

    float intermediateX = sin((uTime)/1000);
	float xPos = intermediateX*Position.x;
	
	float intermediateY = sin(uTime/1000);
	float yPos = intermediateY*Position.y;

	gl_Position = vec4(xPos, yPos, Position.z, 1.0);
	Colour = vColour; 					//pass the vertex colour unchanged to the fragment shader

}