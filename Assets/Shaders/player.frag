#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec2 textureCoordinate; //tex coords from vertex shader
in vec3 normals;
in vec3 fragmentPosition;



uniform sampler2D aTex;		//uniform holding texture info from main programme
uniform int LightingType;
uniform float uTime;

uniform vec3 lightCol1;
uniform vec3 lightPos1; 
uniform vec3 lightPos2;

void main()
{
	//ambient component
	//********************************
	//set the ambient coeff from material
	float lightAmbientStrength = 0.3f;
	vec3 objectAmbientReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = (lightAmbientStrength * objectAmbientReflectionCoeff) * lightCol1;
	
	//diffuse component
	//********************************
	//normalise normal vectors (reset them as unit vectors)
	vec3 nNormal = normalize(normals);

	//calculate the light direction from the light position and the fragment position
    vec3 lightDirection1 = normalize(lightPos1 - fragmentPosition);
	vec3 lightDirection2 = normalize(lightPos2 - fragmentPosition);

	//determine the dot product of normal direction and light direction
	float diffuseStrength1 = max(dot(nNormal, lightDirection1), 0.0f);
	float diffuseStrength2 = max(dot(nNormal, lightDirection2), 0.0f);


	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse1 = (diffuseStrength1 * objectDiffuseReflectionCoeff) * lightCol1;
	vec3 diffuse2 = (diffuseStrength2 * objectDiffuseReflectionCoeff) * lightCol1;

	//===================
	//Colour Changing Stuff
	//===================


	vec4 textureColour = texture(aTex, textureCoordinate);

	//==========================
	//Attenuation===============
	//==========================
	float attenuation1 = length(lightPos1 - fragmentPosition);
	float attenuation2 = length(lightPos2 - fragmentPosition);


	// CHANGES TO LIGHTING TYPE


	if (LightingType == 0)
	{
	vertColour = (vec4((ambient+ (diffuse1/attenuation1)+(diffuse2/attenuation2)),1.0) * textureColour);
	}
	else if (LightingType == 1)
	{
	vertColour = (vec4((ambient+ diffuse1+ diffuse2),1.0) * textureColour);
	}
	else if (LightingType == 2)
	{
	vertColour = (vec4((ambient),1.0) * textureColour);
	}
	
}