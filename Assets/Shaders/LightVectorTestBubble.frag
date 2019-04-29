#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec2 textureCoordinate; //tex coords from vertex shader
in vec3 normals;
in vec3 fragmentPosition;



uniform sampler2D aTex;		//uniform holding texture info from main programme
uniform int LightingType;
uniform float uTime;

uniform vec3 lightCol1;
uniform vec3 lightCol2;
uniform vec3 lightCol3;

uniform vec3 lightPos1; 
uniform vec3 lightPos2;
uniform vec3 lightPos3;

uniform vec3 camPos;

void main()
{
	//ambient component
	//********************************
	//set the ambient coeff from material
	//********************************

	float lightAmbientStrength = 0.3f;
	vec3 objectAmbientReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = (lightAmbientStrength * objectAmbientReflectionCoeff) * lightCol1;


	
	//diffuse component
	//********************************
	//normalise normal vectors (reset them as unit vectors)
	//********************************

	vec3 nNormal = normalize(normals);


	//calculate the light direction from the light position and the fragment position

    vec3 lightDirection1 = normalize(lightPos1 - fragmentPosition);
	vec3 lightDirection2 = normalize(lightPos2 - fragmentPosition);
	vec3 lightDirection3 = normalize(lightPos3 - fragmentPosition);

	//determine the dot product of normal direction and light direction

	float diffuseStrength1 = max(dot(nNormal, lightDirection1), 0.0f);
	float diffuseStrength2 = max(dot(nNormal, lightDirection2), 0.0f);
	float diffuseStrength3 = max(dot(nNormal, lightDirection3), 0.0f);


	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse1 = (diffuseStrength1 * objectDiffuseReflectionCoeff) * lightCol1;
	vec3 diffuse2 = (diffuseStrength2 * objectDiffuseReflectionCoeff) * lightCol2;
	vec3 diffuse3 = (diffuseStrength3 * objectDiffuseReflectionCoeff) * lightCol3;

	//===================
	//Colour Changing Stuff
	//===================

	float r = 1.0 + (sin(uTime * 0.001 + 0.0) / 2.0);
	float g = 1.0 + (sin(uTime * 0.001 + 2.0) / 2.0);
	float b = 1.0 + (sin(uTime * 0.001 + 4.0) / 2.0);

	vec4 col = vec4(r, g, b, 1.0);

	vec4 textureColour = texture(aTex, textureCoordinate +uTime*0.00005) * col;


	//==========================
	//Attenuation===============
	//==========================
	float attenuation1 = length(lightPos1 - fragmentPosition);
	float attenuation2 = length(lightPos2 - fragmentPosition);
	float attenuation3 = length(lightPos3 - fragmentPosition);


	//**********************
	// Specular phong model
	//***********************
	float specularIntensity = 0.1;
	
	vec3 viewDir = normalize(camPos - fragmentPosition);
	vec3 reflectionDir1 = reflect(-lightDirection1, nNormal);
	vec3 reflectionDir2 = reflect(-lightDirection2, nNormal);
	vec3 reflectionDir3 = reflect(-lightDirection3, nNormal);
	
	float specularCalc1 = pow(max(dot(camPos, reflectionDir1), 0.0), 1);
	float specularCalc2 = pow(max(dot(camPos, reflectionDir2), 0.0), 1);
	float specularCalc3 = pow(max(dot(camPos, reflectionDir3), 0.0), 1);
	
	vec3 specular1 = specularIntensity * specularCalc1 * lightCol1;  
	vec3 specular2 = specularIntensity * specularCalc2 * lightCol2; 
	vec3 specular3 = specularIntensity * specularCalc3 * lightCol3; 

	// CHANGES TO LIGHTING TYPE
	   	  

	if (LightingType == 0)
	{
	vertColour = (vec4((ambient + (diffuse1/attenuation1) + (diffuse2/attenuation2) + (diffuse3/(attenuation3*2))+specular1+specular2+specular3),1.0) * textureColour);
	}	
	if (LightingType == 1)
	{
	vertColour = (vec4((ambient + (diffuse1/attenuation1) + (diffuse2/attenuation2) + (diffuse3/(attenuation3*2))),1.0) * textureColour);
	}	
	else if (LightingType == 2)																					  
	{
	vertColour = (vec4((ambient+ diffuse1+ diffuse2 + diffuse3),1.0) * textureColour);
	}
	else if (LightingType == 3)
	{
	vertColour = (vec4((ambient),1.0) * textureColour);
	}
	
}