#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sdl.h"

class Camera
{
public:
	static glm::mat4 viewMatrix;
	static glm::mat4 projectionMatrix;

	static void INIT()
	{
		viewMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
	};

private:

};


//set default values for view and projection stuff
glm::mat4 Camera::viewMatrix = glm::mat4(1.0f);
//--
glm::mat4 Camera::projectionMatrix = glm::mat4(1.0f);