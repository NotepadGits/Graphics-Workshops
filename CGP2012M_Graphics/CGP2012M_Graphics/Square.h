#pragma once
#include <GL/glew.h>
#include <array>

class Square
{
public:
	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	GLuint EBO;

	//shader for the square
	Shader vSh, fSh;
	GLuint shaderProgram;
	
	//constructor
	Square() 
	{
		//shaders
		vSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.vert");
		fSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.frag");

		vSh.getShader(1);
		fSh.getShader(2);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vSh.shaderID);
		glAttachShader(shaderProgram, fSh.shaderID);
		glLinkProgram(shaderProgram);

		glDeleteShader(vSh.shaderID);
		glDeleteShader(fSh.shaderID);
	};
	
	//define vertices for the square
	GLfloat sq_vertices[32] = {
		//vertices				colour				texture coordinates    
		0.1f, 0.1f, 0.0f,		 0.0f, 1.0f, 0.0f,   1.0f, 1.0f,		
		0.1f, -0.1f, 0.0f,		 0.0f, 1.0f, 0.0f,	 1.0f, 0.0f,
		-0.1f, -0.1f, 0.0f,		0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-0.1f, 0.1f, 0.0f,		 0.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	//define indices for the square
	GLuint indices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	void setBuffers()
	{
		//
		//OpenGL buffers
		//set up 1 for the triangle
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);
		//initialise the index buffer
		glGenBuffers(1, &EBO);
		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sq_vertices), sq_vertices, GL_STATIC_DRAW);
		//set up the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		// Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Then set our colour data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		
		//set the texture coordinates attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		

		//Unbind the VAO
		glBindVertexArray(0);

	}

	void render()
	{
		//draw the square 
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

};
