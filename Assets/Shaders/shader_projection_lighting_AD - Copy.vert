#version 440 core
layout (location = 0) in vec3 Position; //vertex positions
//layout (location=1) in vec3 vertexColours;  //vertex colours
layout (location=1) in vec2 texCoord;	//tex coords
layout (location=2) in vec3 normal;	// vertex normals

 						
out vec2 textureCoordinate;
out vec3 normals;
out vec3 fragmentPosition;
out vec3 lightColour;
out vec3 lightPosition;

uniform mat4 uNormalMatrix;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 lightCol;
uniform vec3 lightPos; 


void main()
{
	
	gl_Position = uProjection * uView * uModel * vec4(Position.x, Position.y, Position.z, 1.0); 					
	textureCoordinate = vec2(texCoord.x, 1 - texCoord.y);
	
	//get the fragment position in world coordinates as this is where the lighting will be calculated
	fragmentPosition = vec3(uModel * vec4(Position, 1.0f));
	
	
	//pass the normals to the fragment shader unmodified
	//normals = normal;
	
	//pass normals to fragment shader after modifying for scaling
	//calculate a 'normal matrix' and multiply by the unmodified normal
	normals = mat3(uNormalMatrix) * normal;
	
	lightColour = lightCol;
	lightPosition = lightPos;

}