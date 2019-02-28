#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"

class Triangle_T
{
public:

	//shader for the triangle
	Shader vSh1, fSh1;
	GLuint shader;

	//single texture for the triangle
	Texture tex;

	//define vertices for the triangle -position/colour/texture coordinates
	GLfloat vertices[24] = {
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,	  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f

	};


	//set up vertex buffer object
	GLuint VBO;

	//set up vertex array object
	GLuint VAO;





	/**
	 * @brief Creates a new instance of the textured triangle class
	 *
	*/
	Triangle_T()
	{
		//Compile, load all the shaders and stuff
		createShaderProgram();

		//Load in the texture to be rendered
		tex.load("..//..//Assets//Textures//carbon-fibre-seamless-texture.jpg");

		//Set the buffers -- upload vertex data and all that jazz
		setBuffers();
	}



	/**
	 * @brief Set the current time in the shader to a value that is passed through parameters
	 *
	*/
	void setCurrentTime(float time)
	{
		//We have to use the program because opengl will crash otherwise
		glUseProgram(shader);

		//Get the id / location of "uTime" variable in this shader
		int timeLocation = glGetUniformLocation(shader, "uTime");

		//Set the "uTime" variable to the value of the time variable (passed by parameter)
		glProgramUniform1f(shader, timeLocation, time);
	}


	/**
	 * @brief Loads in a frag/vert shaders from a file and compiles it to
	 *        a shader program
	 *
	*/
	void createShaderProgram()
	{
		//Shaders
		vSh1.shaderFileName("..//..//Assets//Shaders//shader_vColour_Texture.vert");
		fSh1.shaderFileName("..//..//Assets//Shaders//shader_vColour_Texture.frag");

		//Get the shaders id and compile them, etc
		vSh1.getShader(1);
		fSh1.getShader(2);

		//Make a new program and tell opengl that 
		//this program will contain the vert/frag shaders
		shader = glCreateProgram();
		glAttachShader(shader, vSh1.shaderID);
		glAttachShader(shader, fSh1.shaderID);
		glLinkProgram(shader);

		//Delete reference to the ids (we dont need them, they're now
		//in the shader program)
		glDeleteShader(vSh1.shaderID);
		glDeleteShader(fSh1.shaderID);
	}
	 

	/**
	 * @brief Set buffers
	 *
	*/
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


	/**
	 * @brief Render function -- renders the textured triangle
	 *
	*/
	void render()
	{
		//Use the shader to render
		glUseProgram(shader);

		//Tell OpenGL we're talking about this texture
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		
		//Bind the VAO so opengl knows we're talking about the stuff
		//we uploaded
		glBindVertexArray(VAO);

		//Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Unbind / tell opengl to forget about what we were talking about
		glBindVertexArray(0);
	}
};
