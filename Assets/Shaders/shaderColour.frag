#version 440 core
out vec4 FragmentColour;
in vec3 vertexColour;

void main()
{
	FragmentColour = vec4(vertexColour, 1.0f);
}