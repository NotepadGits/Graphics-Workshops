#pragma once
#include <GL/glew.h>
#include <array>
#include <cmath>

class Circle
{
public:

	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	GLuint EBO;

	//set up vertex array
	GLfloat vertices[180];
	//set up index array
	GLuint indices[87] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 9,
		0, 9, 10,
		0, 10, 11,
		0, 11, 12,
		0, 12, 13,
		0, 13, 14,
		0, 14, 15,
		0, 15, 16,
		0, 16, 17,
		0, 17, 18,
		0, 18, 19,
		0, 19, 20,
		0, 20, 21,
		0, 21, 22,
		0, 22, 23,
		0, 23, 24,
		0, 24, 25,
		0, 25, 26,
		0, 26, 27,
		0, 27, 28,
		0, 28, 29
	};


	Circle(float radius, float offsetX, float offsetY)
	{
		//origin of circle at 0,0,0
		vertices[0] = 0.0f + offsetX;
		vertices[1] = 0.0f + offsetY;
		vertices[2] = 0.0f;
		//colour of origin vertex
		vertices[3] = 0.0f;
		vertices[4] = 0.0f;
		vertices[5] = 0.1f;

		GLfloat angle = 0.0f;

		//set remaining vertices based on radius
		for (int i = 6; i < 180; i += 6)
		{
			vertices[i] = (radius * cos(angle)) + offsetX;
			vertices[i + 1] = (radius * sin(angle)) + offsetY;
			vertices[i + 2] = 0.0f;
			//colour information
			vertices[i + 3] = 0.8f;
			vertices[i + 4] = 0.0f;
			vertices[i + 5] = 0.4f;
	
			//increase angle value in radians
			//(2*pi)/number of verts on circumference
			angle += (2*3.141)/28.0f;

		}
	}

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//set up the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//colour information
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//Unbind the VAO
		glBindVertexArray(0);
	}

	void render()
	{
		//draw the circle 
		glBindVertexArray(VAO);
		glPointSize(5.0f);
		glDrawElements(GL_TRIANGLES, 87, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};
