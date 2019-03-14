/**
* @file   FontTextureClass.h
* @author Benjamin Williams <bwilliams@lincoln.ac.uk>
* @brief  A class, extending from the Texture class, which supports font renderering.
*/

#pragma once

//Include the texture class
#include "TextureClass.h"

/**
* A class for a font texture. A font texture is basically a texture, but
* we have to do some extra jiggery-pokery to get SDL to work with OpenGL
* properly.
*/
class FontTextureClass : public Texture
{

public:

	FontTextureClass()
	{
		tex = nullptr;
	}

	/**
	* Given an SDL_Surface, sets the texture instance
	*/
	void setTex(SDL_Surface* tex)
	{
		//Set the texture, then call setBuffers again to
		//re-upload it
		this->tex = tex;
		this->setBuffers();
	}

	
	/**
	* Sets the buffer data for this texture.
	*/
	void setBuffers()
	{
		//Is texture null? If it is.. then return. We don't
		//want to call functions on something that is null
		if (!tex)
			return;

		//Allocate space for the texture, and bind using this generated
		//texture id
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);

		//Set up texture parameters -- texture wrap mode is clamp, and interpolation
		//is bilinear interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Upload using GL_RGBA, unbind the texture
		if (tex->format->BytesPerPixel == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);

		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);

		glBindTexture(GL_TEXTURE_2D, 0);

		//And free the surface! It's all uploaded :)
		SDL_FreeSurface(tex);
	}
};