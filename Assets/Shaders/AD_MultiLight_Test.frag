#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec2 textureCoordinate; //tex coords from vertex shader
in vec3 normals;
in vec3 fragmentPosition;
in vec3 lightColour;
in vec3 lightPosition;
in vec3 lightPosition2;


uniform sampler2D aTex;		//uniform holding texture info from main programme
uniform int LightingType;
uniform float uTime;

void main()
{
	//ambient component
	//********************************
	//set the ambient coeff from material
	float lightAmbientStrength = 0.3f;
	vec3 objectAmbientReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = (lightAmbientStrength * objectAmbientReflectionCoeff) * lightColour;
	
	//diffuse component
	//********************************
	//normalise normal vectors (reset them as unit vectors)
	vec3 nNormal = normalize(normals);

	//calculate the light direction from the light position and the fragment position
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	vec3 lightDirection2 = normalize(lightPosition2 - fragmentPosition);

	//determine the dot product of normal direction and light direction
	float diffuseStrength = max(dot(nNormal, lightDirection), 0.0f);
	float diffuseStrength2 = max(dot(nNormal, lightDirection2), 0.0f);


	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse = (diffuseStrength * objectDiffuseReflectionCoeff) * lightColour;
	vec3 diffuse2 = (diffuseStrength2 * objectDiffuseReflectionCoeff) * lightColour;

	//===================
	//Colour Changing Stuff
	//===================

	float r = 1.0 + (sin(uTime * 0.001 + 0.0) / 2.0);
	float g = 1.0 + (sin(uTime * 0.001 + 2.0) / 2.0);
	float b = 1.0 + (sin(uTime * 0.001 + 4.0) / 2.0);

	vec4 col = vec4(r, g, b, 1.0);

	vec4 textureColour = texture(aTex, textureCoordinate) * col;

	//==========================
	//Attenuation===============
	//==========================
	float attenuation = length(lightPosition - fragmentPosition);
	float attenuation2 = length(lightPosition2 - fragmentPosition);


	// CHANGES TO LIGHTING TYPE


	if (LightingType == 0)
	{
	vertColour = (vec4((ambient+ (diffuse/attenuation)+(diffuse2/attenuation2)),1.0) * textureColour);
	}
	else if (LightingType == 1)
	{
	vertColour = (vec4((ambient+ diffuse+ diffuse2),1.0) * textureColour);
	}
	else if (LightingType == 2)
	{
	vertColour = (vec4((ambient),1.0) * textureColour);
	}
	
}