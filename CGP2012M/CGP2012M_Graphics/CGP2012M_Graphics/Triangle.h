#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>

class Triangle
{
public:
	

	//constructor
	Triangle()
	{
	}

	//define vertices for the triangle
	GLfloat vertices[9] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,	
		0.0f, 0.0f, 0.0f

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		////colours
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(1);
		////texture coords
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(2);
		//Unbind the VAO
		glBindVertexArray(0);
	}

	void render()
	{
		//draw the triangle 
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
};
