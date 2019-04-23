#pragma once
#include <GL/glew.h>
#include "TextureClass.h"
#include <array>
#include <cmath>
#include "ShaderClass.h"
#include "Camera.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "ModelLoaderClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Circle3D
{
public:
	//shaders
	Shader vSh, fSh;
	GLuint shaderProgram;

	//interleaved vertex, texture, normal data
//more efficient than separate VBO's
	std::vector<GLfloat> modelData;

	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;


	//set up texture
	Texture tex;
	Texture tex1;
	Texture tex2;
	Texture tex3;


	//MVP matrix
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0);
	glm::mat4 rotation = glm::mat4(1.0);

	float xPos;
	float yPos;

	float radius;
	float xSpeed = 0, ySpeed = 0.0, zSpeed = 0;
	glm::vec3 velocity = glm::vec3(xSpeed, ySpeed, zSpeed);
	glm::vec3 position;

	float timeTemp;
	float radiusTemp;
	//set up index array for stiching
	//vertex/texture coord/normal containers for the model
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;



	Circle3D(float radius ,char* file)
	{
		this->radius = radius;

		ModelImport importer;
		importer.LoadOBJ2(file, vertices, texCoords, normals, indices);


		timeTemp = SDL_GetTicks();

		///Spawn Direction velocity
		srand(time(0));
		float rVal = rand() % 10;
		xSpeed = rVal /= 1000;
		rVal = rand() % 1;
		if (rVal == 0) xSpeed = -xSpeed;
		rVal = rand() % 10;
		ySpeed = rVal /= 1000;
		rVal = rand() % 1;
		if (rVal == 0) ySpeed = -ySpeed;



		translate = glm::translate(translate, glm::vec3(0.5, 0.5, 0));

		position = glm::vec3(translate[3]);

		createShaders();

		//generateVerts(radius);

		loadTex();

		setBuffers();

	};
	glm::vec3 lightColour{ 1,1,1 };
	glm::vec3 lightPosition{ 1,1,1 };
	GLuint elapsedTime;
	glm::mat4 normalMatrix;

	void passMatricesToShader()
	{
		elapsedTime = SDL_GetTicks();
		glUseProgram(shaderProgram);
		//lighting uniforms
		//get and set light colour and position uniform
		int lightColLocation = glGetUniformLocation(shaderProgram, "lightCol");
		glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour));
		int lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPos");
		glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
		//rotation
		rotation = glm::rotate(rotation, (float)elapsedTime / 2000, glm::vec3(0.0f, 1.0f, 0.0f));
		int importModelLocation = glGetUniformLocation(shaderProgram, "uModel");
		glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(translate*rotation*scale));
		int importViewLocation = glGetUniformLocation(shaderProgram, "uView");
		glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(Camera::viewMatrix));
		 int importProjectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
		glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(Camera::projectionMatrix));
		//set the normal matrix to send to the vertex shader
		//Light calculations take place in model-view space
		//So we calculate the normal matrix in that space
		normalMatrix = glm::transpose(glm::inverse(translate*rotation*scale * Camera::viewMatrix));
		//set the normalMatrix in the shaders
		glUseProgram(shaderProgram);
		int normalMatrixLocation = glGetUniformLocation(shaderProgram, "uNormalMatrix");
		glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glBindTexture(GL_TEXTURE_2D, tex.texture);
	}

	glm::mat4 getModelMatrix()
	{
		return translate * rotation * scale;
	}



	void createShaders() {
		//shaders

		vSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.vert");
		fSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.frag");

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
		tex.load("..//..//Assets//Textures//bubble.png");
		tex1.load("..//..//Assets//Textures//bubble_1.png");
		tex2.load("..//..//Assets//Textures//bubble_2.png");
		tex3.load("..//..//Assets//Textures//bubble_3.png");
	};

	void setBuffers()
	{
		////interleave the vertex/texture/normal data
		for (int i = 0; i < indices.size(); i += 3)
		{
			modelData.push_back(vertices[indices[i]].x);
			modelData.push_back(vertices[indices[i]].y);
			modelData.push_back(vertices[indices[i]].z);
			modelData.push_back(texCoords[indices[i + 1]].x);
			modelData.push_back(texCoords[indices[i + 1]].y);
			modelData.push_back(normals[indices[i + 2]].x);
			modelData.push_back(normals[indices[i + 2]].y);
			modelData.push_back(normals[indices[i + 2]].z);

		}
		std::cout << sizeof(modelData[0]) << std::endl;
		std::cout << modelData.size() << std::endl;
		//
		//OpenGL buffers
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);
		//initialise the index buffer
		//glGenBuffers(1, &this->EBO);

		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//buffer size from vector: vector size (number of elements) * size of one element
		glBufferData(GL_ARRAY_BUFFER, modelData.size() * sizeof(modelData[0]), &modelData[0], GL_STATIC_DRAW);
		// Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//set the texture attribute pointer
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//set normal attrib pointer
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		//set up the EBO

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);

		//Unbind the VAO
		glBindVertexArray(0);
	};


	void update()
	{



		position = glm::vec3(translate[3]);
		//glm::vec3 scalar = glm::vec3(scale[3]);

		float scaleValue = scale[0].x;

		float r = (radius / 2) * (scaleValue / 2);


		if (position.x > 3.8 - r)
			xSpeed = -xSpeed;

		if (position.x < 0.2 + r)
			xSpeed = -xSpeed;

		if (position.y > 2.8 - r)
			ySpeed = -ySpeed;

		if (position.y < 0.2 + r)
			ySpeed = -ySpeed;


		velocity = glm::vec3(xSpeed, ySpeed, 0);


		translate = glm::translate(translate, velocity);






		//std::cout << position.x << std::endl;
	}

	void render()
	{
		//draw the circle 
		glBindVertexArray(VAO);

		passMatricesToShader();

		/*if (SDL_GetTicks() % 1000 > 0 && SDL_GetTicks() % 1000 < 249)
		{
			glBindTexture(GL_TEXTURE_2D, tex.texture);
		}
		else if (SDL_GetTicks() % 1000 > 250 && SDL_GetTicks() % 1000 < 499)
		{
			glBindTexture(GL_TEXTURE_2D, tex1.texture);
		}
		else if (SDL_GetTicks() % 1000 > 500 && SDL_GetTicks() % 1000 < 749)
		{
			glBindTexture(GL_TEXTURE_2D, tex2.texture);
		}
		else if (SDL_GetTicks() % 1000 > 750 && SDL_GetTicks() % 1000 < 999)
		{
			glBindTexture(GL_TEXTURE_2D, tex3.texture);
		}*/




		glPointSize(1.0f);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vertices));
		glBindVertexArray(0);
	};
};

