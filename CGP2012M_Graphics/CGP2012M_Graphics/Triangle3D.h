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

#include "Light.h"
class Triangle3D
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

	float angle = 0;
	//MVP matrix
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0f);
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



	Triangle3D(float radius, char* file)
	{
		this->radius = radius;

		createShaders();

		ModelImport importer;
		importer.LoadOBJ2(file, vertices, texCoords, normals, indices);

		loadTex();
		setBuffers();

		translate = glm::translate(translate, glm::vec3(0.5, 0.5, 0));

		position = glm::vec3(translate[3]);

		scale = glm::scale(scale, glm::vec3(0.2f));
	};
	GLuint elapsedTime;
	glm::mat4 normalMatrix;


	void passMatricesToShader()
	{
		elapsedTime = 100;
		glUseProgram(shaderProgram);
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightCol1"), 1, glm::value_ptr(Light::lightCol1));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightCol2"), 1, glm::value_ptr(Light::lightCol2));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightCol3"), 1, glm::value_ptr(Light::lightCol3));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos1"), 1, glm::value_ptr(Light::lightPosition1));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos2"), 1, glm::value_ptr(Light::lightPosition2));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos3"), 1, glm::value_ptr(Light::lightPosition3));
		glUniform3fv(glGetUniformLocation(shaderProgram, "camPos"), 1, glm::value_ptr(Camera::position));

		glUniform1i(glGetUniformLocation(shaderProgram, "LightingType"), Camera::lightingType);
		glUniform1f(glGetUniformLocation(shaderProgram, "uTime"), SDL_GetTicks() + timeTemp);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModel"), 1, GL_FALSE, glm::value_ptr(translate*rotation*scale));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uView"), 1, GL_FALSE, glm::value_ptr(Camera::viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, glm::value_ptr(Camera::projectionMatrix));
		normalMatrix = glm::transpose(glm::inverse(translate*rotation*scale * Camera::viewMatrix));

		glUseProgram(shaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glBindTexture(GL_TEXTURE_2D, tex.texture);
	}

	glm::mat4 getModelMatrix()
	{
		return translate * rotation * scale;
	}



	void createShaders() {
		//shaders

		///vSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.vert");
		///fSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.frag");
		vSh.shaderFileName("..//..//Assets//Shaders//LightVectorTest.vert");
		fSh.shaderFileName("..//..//Assets//Shaders//LightVectorTest.frag");

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
		tex.load("..//..//Assets//Textures//carbon-fibre-seamless-texture.jpg");
		tex.setBuffers();
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

		//glm::vec3 scalar = glm::vec3(scale[3]);

		float scaleValue = scale[0].x;

		float r = (radius / 2) * (scaleValue / 2);








		//std::cout << position.x << std::endl;
	}

	void render(Lives &lives)
	{

		glm::vec3 position = glm::vec3(translate[3]);

		if (position.x > 3.8)
		{
			lives.LivesRemaining -= 1;
			translate = glm::translate(translate, glm::vec3(-1.5, 0, 0));
		}
		if (position.x < 0.2)
		{
			lives.LivesRemaining -= 1;
			translate = glm::translate(translate, glm::vec3(1.5, 0, 0));
		}
		if (position.y > 2.8)
		{
			lives.LivesRemaining -= 1;
			translate = glm::translate(translate, glm::vec3(0, -1.5, 0));
		}
		if (position.y < 0.2)
		{
			lives.LivesRemaining -= 1;
			translate = glm::translate(translate, glm::vec3(0, 1.5, 0));
		}
		//draw the circle 
		glBindVertexArray(VAO);

		passMatricesToShader();
		//glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, tex.texture);

		//glPointSize(1.0f);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vertices));
		glBindVertexArray(0);
	};
};

