/**
 * @file   Text.h
 * @author Benjamin Williams <bwilliams@lincoln.ac.uk>
 * @brief  A class which enables you to draw text to the screen with OpenGL.
*/

#pragma once

//Standard libraries
#include <array>
#include <ctime>

//Import opengl and SDL stuff
#include <GL/glew.h>
#include <glm/glm.hpp>
//--
#include "SDL.h"
#include "SDL_ttf.h"

//Project-related dependencies
#include "ShaderClass.h"
#include "FontTextureClass.h"


//Some macros to make OpenGL easier on the eyes :)
#define GL_FLOAT_STRIDE(x) (x * sizeof(GLfloat)) 
#define GL_FLOAT_POINTER(x)   (GLvoid*)GL_FLOAT_STRIDE(x)


/**
 * Text align modes -- left alignment, centre, and right
*/
enum ALIGN_MODE
{
	ALIGN_MODE_CENTER,
	ALIGN_MODE_LEFT,
	ALIGN_MODE_RIGHT
};


/**
 * A class which represents text in OpenGL. Uses sdl2_ttf to render
 * text to a texture, and renders it on a plane.
*/
class Text
{
public:

	//Frag and vertex shaders, and the shader program
	//which is the frag + vert
	Shader vert, frag;
	GLuint shaderProgram;

	//The font texture loading class, this will be used 
	//for our final renderered text
	FontTextureClass tex;

	//The font we're going to use
	TTF_Font* font;

	//The text that is going to be rendered
	const char* text;

	//The offset (in world space) to offset the text at
	glm::vec2 offset;

	//And the alignment mode of the rendered text
	unsigned int alignMode = ALIGN_MODE_LEFT;

	//The VBO data: (positions, uvs)
	GLfloat data[4 * (3 + 2)] =
	{
		//pos        u  v
		0, 0, 0,     0, 0,  // bl
		0, 1, 0,     0, 1,  // tl
		1, 1, 0,     1, 1,  // tr
		1, 0, 0,     1, 0   // br
	};

	//The EBO data -- indices (there's just two triangles for this plane)
	GLubyte indices[3 * 2] =
	{
		0, 1, 2,
		0, 2, 3
	};

	//set up vertex buffer object
	GLuint VAO, VBO, EBO;

	/**
	* Constructs a new text instance
	*/
	Text(const char* fontPath, int size = 128, float offsetX = -1, float offsetY = -1, unsigned int alignMode = ALIGN_MODE_LEFT) 
		: offset(glm::vec2(offsetX, offsetY)), alignMode(alignMode)
	{
		//Is TTF initialised? If not, initialise it
		if(!TTF_WasInit())
			TTF_Init();

		//Load in the relevant shaders
		vert.shaderFileName("..//..//Assets//Shaders//shader_Text.vert");
		frag.shaderFileName("..//..//Assets//Shaders//shader_Text.frag");

		//Get them
		vert.getShader(1);
		frag.getShader(2);

		//Create the shaders and link them into a shader program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vert.shaderID);
		glAttachShader(shaderProgram, frag.shaderID);
		glLinkProgram(shaderProgram);

		//Delete the individual shaders because they're in the program now
		glDeleteShader(vert.shaderID);
		glDeleteShader(frag.shaderID);

		//Load the given font
		this->loadFont(fontPath, size);

		//And set buffers for this object
		this->setBuffers();
	}

	/**
	* Sets the text alignment
	*/
	void setAlignment(unsigned int alignMode)
	{
		//Set the alignment mode and update the vertices with the given text
		this->alignMode = alignMode;
		this->updateVertices(this->text);
	}

	/**
	* Loads a font from a string path, at a given font size
	*/
	void loadFont(const char* path, int ptsize = 48) 
	{
		//Open the font
		this->font = TTF_OpenFont(path, ptsize);

		//Is it loaded? If not, throw an error
		if (!this->font)
			throw std::exception("couldn't open up font from path");
	}

	/**
	* Sets the buffers for this object
	*/
	void setBuffers()
	{
		//Allocate space for the per-vertex data (pos and uvs)
		glGenBuffers(1, &VBO);

		//Allocate space for a VAO and EBO, VAO will map VBO inputs to the shader
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);

		//We're gonna be talking about this VAO we just made
		glBindVertexArray(VAO);


		// ebo
		// ----

		//We're gonna be talking about this EBO which belongs inside this VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//Upload the indices
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// ----


		// vbo
		// ----

		//We're gonna be talking about the per-vertex data given to the shader, the vbo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//Upload the vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		//The first three numbers are actually a vec3 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_FLOAT_STRIDE(5), GL_FLOAT_POINTER(0));
		glEnableVertexAttribArray(0);

		//The next two numbers are actually a vec2
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, GL_FLOAT_STRIDE(5), GL_FLOAT_POINTER(3));
		glEnableVertexAttribArray(1);

		// ----


		//We're not talking about the vao anymore
		glBindVertexArray(0);

		//texture buffers
		tex.setBuffers();
	}

	/**
	* Updates the vertices of the plane, given a piece of text
	*/
	void updateVertices(const char* text)
	{
		//Get the current window, from the ctx
		auto window = SDL_GL_GetCurrentWindow();

		//Get the window size
		int winW, winH;
		SDL_GetWindowSize(window, &winW, &winH);

		//Get the size of the rendered text
		int texW, texH;
		TTF_SizeText(font, text, &texW, &texH);

		//The width of the text, scaled to the viewport size
		float w = texW / (1.0 * winW);
		float h = texH / (1.0 * winH);

		//The align X offset is none by default (left)
		float alignOffset = 0;

		//If it's centred, we need to take off half of the width off
		if (alignMode == ALIGN_MODE_CENTER)
			alignOffset = -((texW / 2.0) / (1.0 * winW));

		//Otherwise, take off the full width
		else if (alignMode == ALIGN_MODE_RIGHT)
			alignOffset = -(texW / (1.0 * winW));

		//Set up new vertices
		data[0] = offset.x + 0 + alignOffset;
		data[1] = offset.y + 0;
		//--
		data[5] = offset.x + 0 + alignOffset;
		data[6] = offset.y + h;
		//--
		data[10] = offset.x + w + alignOffset;
		data[11] = offset.y + h;
		//--
		data[15] = offset.x + w + alignOffset;
		data[16] = offset.y + 0;

		//And upload the vertex data.. first, we're talking about this vao
		glBindVertexArray(VAO);

		//Then we're gonna upload stuff to this vbo thing we made earlier
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		//And now we're not talking about this vao anymore
		glBindVertexArray(0);
	}

	/**
	* Sets the text to a given piece of text, with a specific colour.
	*/
	void setText(const char* text, int r, int g, int b)
	{
		//Set the text string
		this->text = text;

		//Colours from SDL -> OpenGL need to be BGR > RGB for some reason?
		SDL_Color col = { b, g, r, 0 };

		//Render the text using sdl2_ttf
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text, col);

		//And set the texture 
		tex.setTex(surf);

		//Then update the plane
		updateVertices(text);
	}


	/**
	* Renders the text on the screen.
	*/
	void render()
	{
		//We wanna enable blending because sdl2_ttf renders with alpha.. use
		//the default blend function (lerp from src color to background using src alpha)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//We wanna use the shader we compiled earlier
		glUseProgram(shaderProgram);

		//We're gonna be talking about the texture we made, and the ebo we made
		glBindTexture(GL_TEXTURE_2D, tex.texture);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//Using the vao, draw the plane
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);

		//And we're not talking about this texture anymore.. so unbind and disable tranparency for
		//the next render cycle
		glBindTexture(GL_TEXTURE_2D, 0);
		//glDisable(GL_BLEND);

	}
};
