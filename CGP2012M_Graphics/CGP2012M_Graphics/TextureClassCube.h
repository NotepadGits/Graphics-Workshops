#pragma once
#include <GL/glew.h>

#include "SDL.h"
#include "SDL_image.h"

#include <iostream>

class TextureCube
{
public:
	SDL_Surface* tex;
	GLuint texture;

	//load textures
	void load(const char* fileName)
	{
		//use SDL image loading function in this case
		tex = IMG_Load(fileName);

		if (tex == NULL)
		{
			//std::cout << "Error loading texture: " << fileName << std::endl;
		}
		else
		{
			//std::cout << "Success loading " << fileName << std::endl;
		}

	}

	//set buffers 
	void setBuffers()
	{
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

		//set texture parameters on currently bound texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (tex->format->BytesPerPixel == 3)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGB, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		SDL_FreeSurface(tex);
	}
};