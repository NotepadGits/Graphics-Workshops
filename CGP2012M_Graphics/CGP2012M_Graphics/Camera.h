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

	static glm::vec3 position;
	static glm::vec3 Target;
	static glm::vec3 cameraUp;

	static void INIT(float w, float h)
	{
		viewMatrix = glm::lookAt(position, Target, cameraUp);
		//projectionMatrix = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
	};

	static void resize(float w, float h)
	{
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
	}

	static void update()
	{
		//Target += glm::vec3(0, 0.001, 0);

		float dist = 5.0f;
		float time = SDL_GetTicks();

		//float x = 0.5 + sin(time * 0.001) * dist;
		//float z = 0.5 + cos(time * 0.001) * dist;

		//position.x = x;
		//position.z = z;

		viewMatrix = glm::lookAt(position, Target, cameraUp);
	}

private:

};


//set default values for view and projection stuff
glm::mat4 Camera::viewMatrix = glm::mat4(1.0f);
//--
glm::mat4 Camera::projectionMatrix = glm::mat4(1.0f);
//--
glm::vec3 Camera::Target = glm::vec3(2, 1.5, 0);
glm::vec3 Camera::position = glm::vec3(2, 1.5, 3.5);
//--
glm::vec3 Camera::cameraUp = glm::vec3(0, 1, 0);