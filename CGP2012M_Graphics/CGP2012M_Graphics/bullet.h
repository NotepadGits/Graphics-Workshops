#pragma once
#include <GL/glew.h>
#include "TextureClass.h"
#include <array>
#include <cmath>
#include "ShaderClass.h"
#include "Camera.h"
#include <cstdio>
#include <cstdlib>
#include "Triangle_T.h"

class bullet
{
public:
	//shaders
	Shader vSh, fSh;
	GLuint shaderProgram;



	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	GLuint EBO;

	//set up texture
	Texture tex;

	//set up vertex array
	GLfloat vertices[240];

	//MVP matrix
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1);
	glm::mat4 rotation = glm::mat4(1.0);

	glm::vec3 position;

	float xPos;
	float yPos;

	float radius;
	float angle;
	float xSpeed = 0, ySpeed = 0.00, zSpeed = 0;
	glm::vec3 velocity = glm::vec3(xSpeed, ySpeed, zSpeed);


	float timeTemp;
	float radiusTemp;
	//set up index array for stiching
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


	void passMatricesToShader()
	{
		glUseProgram(shaderProgram);
		//set projection matrix uniform and other triangle values
		int projectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(Camera::projectionMatrix));

		glUniform1f(glGetUniformLocation(shaderProgram, "uTime"), SDL_GetTicks() + timeTemp);



		int modelLocation = glGetUniformLocation(shaderProgram, "uModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(translate*rotation*scale));

		int viewLocation = glGetUniformLocation(shaderProgram, "uView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(Camera::viewMatrix));
	}

	glm::mat4 getModelMatrix()
	{
		return translate * rotation * scale;
	}



	bullet(Triangle_T* plr)
	{
		angle = plr->angle;

		position = glm::vec3(translate[3]);
		glm::vec3 plrposition = glm::vec3(plr->translate[3]);

		this->radius = 0.08f;


		timeTemp = SDL_GetTicks();



		//translate 1 on x
		translate = glm::translate(translate, plrposition);

		//scale by 2 (on xy and z)


		//scale = glm::scale(scale, glm::vec3(radiusTemp));

		//rotate by .. something on the z axis
		//rotation = glm::rotate(rotation, 0.5f, glm::vec3(0, 0, 1));

		createShaders();

		generateVerts(radius);

		loadTex();

		setBuffers();

	};
	void generateVerts(float radius)
	{
		//origin of circle at 0,0,0
		vertices[0] = 0.0f;
		vertices[1] = 0.0f;
		vertices[2] = 0.0f;
		//colour of origin vertex
		vertices[3] = 0.0f;
		vertices[4] = 0.0f;
		vertices[5] = 0.1f;
		//texture coordinates of centre
		vertices[6] = 0.5f;
		vertices[7] = 0.5f;

		GLfloat angle = 0.0f;

		//set remaining vertices based on radius
		for (int i = 8; i < 240; i += 8)
		{
			vertices[i] = (radius * cos(angle));
			vertices[i + 1] = (radius * sin(angle));
			vertices[i + 2] = 0.0f;
			//colour information
			vertices[i + 3] = 0.8f;
			vertices[i + 4] = 0.0f;
			vertices[i + 5] = 0.4f;
			//texture coordinate information
			vertices[i + 6] = ((radius * cos(angle))*2.5f) + 0.5f;
			vertices[i + 7] = ((radius * sin(angle))*2.5f) + 0.5f;

			//increase angle value in radians
			//(2*pi)/number of verts on circumference
			angle += (2 * 3.141) / 28.0f;

		}
	};

	void createShaders() {
		//shaders
		//vSh.shaderFileName("..//..//Assets//Shaders//shader_vColour_Projection.vert");
		//fSh.shaderFileName("..//..//Assets//Shaders//shader_vColour_Projection.frag");

		vSh.shaderFileName("..//..//Assets//Shaders//shader2.vert");
		fSh.shaderFileName("..//..//Assets//Shaders//shader2.frag");

		vSh.getShader(1);
		fSh.getShader(2);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vSh.shaderID);
		glAttachShader(shaderProgram, fSh.shaderID);
		glLinkProgram(shaderProgram);

		glDeleteShader(vSh.shaderID);
		glDeleteShader(fSh.shaderID);
	};

	void loadTex()
	{

		//load the texture file
		tex.load("..//..//Assets//Textures//bullet.png");

	};

	void setBuffers()
	{
		//
		//OpenGL buffers
		//set up 1 for the circle
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//colour information
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//texture information
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		//Unbind the VAO
		glBindVertexArray(0);

		//texture buffers
		tex.setBuffers();

	};


	void update()
	{


		translate = glm::translate(translate, glm::vec3((float)cos(angle)*0.04f, (float)sin(angle)*0.04f, 0.0f));


		position = glm::vec3(translate[3]);

		//std::cout << position.x << std::endl;
	}

	void render()
	{
		//draw the circle 
		glBindVertexArray(VAO);

		passMatricesToShader();

			glBindTexture(GL_TEXTURE_2D, tex.texture);


		glPointSize(5.0f);
		glDrawElements(GL_TRIANGLES, 87, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	};
};
