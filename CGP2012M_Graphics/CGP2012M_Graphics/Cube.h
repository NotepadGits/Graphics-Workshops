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
class Cube
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



	Cube(char* file)
	{
		createShaders();

		ModelImport importer;
		importer.LoadOBJ2(file, vertices, texCoords, normals, indices);

		loadTex();
		setBuffers();

		timeTemp = SDL_GetTicks();

		translate = glm::translate(translate, glm::vec3(0.5, 0.5, 0));

		position = glm::vec3(translate[3]);


	};



	glm::vec3 lightColour{ 1.0f, 1.0f, 0.98f };
	glm::vec3 lightPosition{ 0.5f, 0.5f, 0.0f };
	glm::vec3 lightPosition2{ 3.5f, 3.0f, 0.0f };
	GLuint elapsedTime;
	glm::mat4 normalMatrix;

	void passMatricesToShader()
	{
		elapsedTime = 100;
		glUseProgram(shaderProgram);
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightCol"), 1, glm::value_ptr(lightColour));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPosition));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos2"), 1, glm::value_ptr(lightPosition2));
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
		vSh.shaderFileName("..//..//Assets//Shaders//player.vert");
		fSh.shaderFileName("..//..//Assets//Shaders//player.frag");

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
		tex.load("..//..//Assets//Textures//plaid.jpg");
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

	};

	void setScale(float x, float y, float z)
	{
		scale = glm::scale(scale, glm::vec3(x, y, z));
	};
	void setTranslate(float x, float y, float z)
	{
		translate = glm::translate(translate, glm::vec3(x, y, z));
	};

	void render()
	{
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

