#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"
#include "Camera.h"
#include "Light.h"

class Square
{
public:

	//Shaders required
	Shader vSh1, fSh1;
	GLuint shader;

	//texture to apply
	Texture tex;

	//verticies for the square/rect
	GLfloat vertices[32] = {
		//vertex			colour			Texture coord
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

	//MVP matrix
	glm::mat4 translate = glm::mat4(1.0);
	glm::mat4 scale = glm::mat4(1.0);
	glm::mat4 rotation = glm::mat4(1.0);

	float xPos;
	float yPos;

	glm::vec3 velocity = glm::vec3(0);






	// Vertext bufferes object
	GLuint VBO;
	//Vertext array object
	GLuint VAO;
	//index buffer object
	GLuint IBO;

	/*
	*@brief  New instance of a textured rect/square
	*
	*/
	Square() {
		translate = glm::translate(translate, glm::vec3(2,1.5, 0));
		scale = glm::scale(scale, glm::vec3(19,14,0));

		//load the sharders and compile
		createShaderProgram();

		//Load textures for rendering
		tex.load("..//..//Assets//Textures//background.png");

		//mount buffers with vertex data
		setBuffers();
	};


	void passMatricesToShader()
	{

		glUseProgram(shader);
		glUniform3fv(glGetUniformLocation(shader, "lightCol1"), 1, glm::value_ptr(Light::lightCol1));
		glUniform3fv(glGetUniformLocation(shader, "lightCol2"), 1, glm::value_ptr(Light::lightCol2));
		glUniform3fv(glGetUniformLocation(shader, "lightCol3"), 1, glm::value_ptr(Light::lightCol3));
		glUniform3fv(glGetUniformLocation(shader, "lightPos1"), 1, glm::value_ptr(Light::lightPosition1));
		glUniform3fv(glGetUniformLocation(shader, "lightPos2"), 1, glm::value_ptr(Light::lightPosition2));
		glUniform3fv(glGetUniformLocation(shader, "lightPos3"), 1, glm::value_ptr(Light::lightPosition3));


		glUniform1i(glGetUniformLocation(shader, "LightingType"), Camera::lightingType);
		glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, glm::value_ptr(translate*rotation*scale));
		glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, glm::value_ptr(Camera::viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader, "uProjection"), 1, GL_FALSE, glm::value_ptr(Camera::projectionMatrix));


		glBindTexture(GL_TEXTURE_2D, tex.texture);
	}

	glm::mat4 getModelMatrix()
	{
		return translate * rotation * scale;
	}

	void createShaderProgram()
	{
		//mount vertex and fragment shaders
		vSh1.shaderFileName("..//..//Assets//Shaders//BACKGROUND.vert");
		fSh1.shaderFileName("..//..//Assets//Shaders//LightVectorTest.frag");

		//give the shaders an ID for compilation
		vSh1.getShader(1);
		fSh1.getShader(2);

		//compile the shaders into a program
		shader = glCreateProgram();
		glAttachShader(shader, vSh1.shaderID);
		glAttachShader(shader, fSh1.shaderID);
		glLinkProgram(shader);

		//clear reference IDs
		glDeleteShader(vSh1.shaderID);
		glDeleteShader(fSh1.shaderID);

	};

	void setCurrentTime(float time)
	{
		//We have to use the program because opengl will crash otherwise
		glUseProgram(shader);

		//Get the id / location of "uTime" variable in this shader
		int timeLocation = glGetUniformLocation(shader, "uTime");

		//Set the "uTime" variable to the value of the time variable (passed by parameter)
		glProgramUniform1f(shader, timeLocation, time);
	}

	void setBuffers() {
		//triangle buffer
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);

		//index buffer (Not sure what this is for)
		glGenBuffers(1, &IBO);

		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//set up the IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
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

		//texture buffers
		tex.setBuffers();
	}


	void render()
	{
		passMatricesToShader();
		//Use the shader to render
		glUseProgram(shader);


		//Tell OpenGL we're talking about this texture
		glBindTexture(GL_TEXTURE_2D, tex.texture);

		//Bind the VAO so opengl knows we're talking about the stuff
		//we uploaded
		glBindVertexArray(VAO);

		//Draw the 2 triangles
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Unbind / tell opengl to forget about what we were talking about
		glBindVertexArray(0);

	}

};
