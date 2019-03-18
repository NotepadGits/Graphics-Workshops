#version 440 core
in vec3 Colour;			//input the Colour values from Vertex shader
in vec2 textureCoordinate; //tex coords from vertex shader

uniform sampler2D aTex;		//uniform holding texture info from main programme
uniform float uTime;

void main()
{

	float r = 1.0 + (sin(uTime * 0.001 + 0.0) / 2.0);
	float g = 1.0 + (sin(uTime * 0.001 + 2.0) / 2.0);
	float b = 1.0 + (sin(uTime * 0.001 + 4.0) / 2.0);

	vec4 col = vec4(r, g, b, 1.0);

	gl_FragColor = texture(aTex, textureCoordinate) * col;
}