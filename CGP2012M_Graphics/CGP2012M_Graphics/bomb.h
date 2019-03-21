#pragma once
#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"
#include "Camera.h"

class Bomb
{
public:

	//Shaders required
	Shader vSh1, fSh1;
	GLuint shader;
	
	//texture to apply
	Texture tex;
	Texture tex1;
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
	bool bomb_used = false;
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
	Bomb() {
		translate = glm::translate(translate, glm::vec3(3.8, 2.8, 0));
		scale = glm::scale(scale, glm::vec3(1, 1, 0));

		//load the sharders and compile
		createShaderProgram();

		//Load textures for rendering
		tex.load("..//..//Assets//Textures//bomb.png");
		tex1.load("..//..//Assets//Textures//bomb1.png");
		//mount buffers with vertex data
		setBuffers();
	};

	void passMatricesToShader()
	{
		glUseProgram(shader);
		//set projection matrix uniform and other triangle values
		int projectionLocation = glGetUniformLocation(shader, "uProjection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(Camera::projectionMatrix));

		glUniform1f(glGetUniformLocation(shader, "uTime"), SDL_GetTicks());



		int modelLocation = glGetUniformLocation(shader, "uModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(translate*rotation*scale));

		int viewLocation = glGetUniformLocation(shader, "uView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(Camera::viewMatrix));
	}

	glm::mat4 getModelMatrix()
	{
		return translate * rotation * scale;
	}

	void createShaderProgram()
	{
		//mount vertex and fragment shaders
		vSh1.shaderFileName("..//..//Assets//Shaders//shader2.vert");
		fSh1.shaderFileName("..//..//Assets//Shaders//shader2.frag");

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
		tex1.setBuffers();
	}


	void render()
	{
		passMatricesToShader();
		//Use the shader to render
		glUseProgram(shader);


		//Tell OpenGL we're talking about this texture
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		if (bomb_used == true) 		glBindTexture(GL_TEXTURE_2D, tex1.texture);

		//Bind the VAO so opengl knows we're talking about the stuff
		//we uploaded
		glBindVertexArray(VAO);

		//Draw the 2 triangles
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Unbind / tell opengl to forget about what we were talking about
		glBindVertexArray(0);

	}

};
