#pragma once
#include <GL/glew.h>
#include <array>
#include <string>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"
#include "Textures.h"

class Triangle_AnyT
{
public:
	//shader for the triangle
	Shader vSh1, fSh1;
	GLuint shaderProgram1;
	//single texture for the triangle
	Texture tex;
	float offsetX, offsetY;


	//constructors
	Triangle_AnyT() {}

	Triangle_AnyT(std::string filename, float _offsetX, float _offsetY)
	{
		offsetX = _offsetX;
		offsetY = _offsetY;

		//shaders
		vSh1.shaderFileName("..//..//Assets//Shaders//shader_vColour_Texture.vert");
		fSh1.shaderFileName("..//..//Assets//Shaders//shader_vColour_Texture.frag");

		vSh1.getShader(1);
		fSh1.getShader(2);

		shaderProgram1 = glCreateProgram();
		glAttachShader(shaderProgram1, vSh1.shaderID);
		glAttachShader(shaderProgram1, fSh1.shaderID);
		glLinkProgram(shaderProgram1);

		glDeleteShader(vSh1.shaderID);
		glDeleteShader(fSh1.shaderID);

		//load the texture file
		tex.load(filename.c_str());

	}

	//define vertices for the triangle -position/colour/texture coordinates
	GLfloat vertices[24] = {
		 -0.5f+offsetX, 0.5f+offsetY, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
		 -0.5f+offsetX, -0.5f+offsetY, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		  0.5f+offsetX, 0.0f+offsetY, 0.0f,   1.0f, 0.0f, 0.0f,    1.0f, 0.5f
	};

	 
	
	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;

	void setBuffers()
	{
		//
		//OpenGL buffers
		//set up 1 for the triangle
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);

		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//colours
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		//Unbind the VAO
		glBindVertexArray(0);

		//texture buffers
		tex.setBuffers();
	}

	void render()
	{
		//draw the triangle 
		//specify the shader program and texture
		glUseProgram(shaderProgram1);
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
};
