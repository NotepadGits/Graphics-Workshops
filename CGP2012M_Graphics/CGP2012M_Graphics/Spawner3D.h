#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"
#include "Camera.h"
#include <vector>
#include "Circle3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Spawner3D
{
public:
	std::vector<Circle3D*> bub;
	float rVal;
	unsigned int x = 0;
	float radius = 0.02f;




	Spawner3D()
	{
		//Circle cr(0.2f);


		//bub.push_back(&cr);
		Circle3D* circle = new Circle3D(0.2f,"..//..//Assets//Models//blenderSphere.obj");

		float r = (rand() / ((float)RAND_MAX));
		r = 0.1 + r * (0.3 - 0.1);

		circle->scale = glm::scale(circle->scale, glm::vec3(r));

		bub.push_back(circle);

		srand(time(0));
	}

	void spawn()
	{

		//(float)radius = rand() % 3 / 10.0f;
		radius = 0.2;

		rVal = rand() % 333;
		if (rVal == 1)
		{
			Circle3D* circle = new Circle3D(radius, "..//..//Assets//Models//blenderSphere.obj");

			float r = (rand() / ((float)RAND_MAX));
			r = 0.1 + r * (0.3 - 0.1);
			radius = r;

			circle->scale = glm::scale(circle->scale, glm::vec3(r));

			bub.push_back(circle);
		}

	}
	void spawnnew()
	{

			Circle3D* circle = new Circle3D(radius, "..//..//Assets//Models//blenderSphere.obj");

			float r = (rand() / ((float)RAND_MAX));
			r = 0.1 + r * (0.3 - 0.1);
			radius = r;

			circle->scale = glm::scale(circle->scale, glm::vec3(r));

			bub.push_back(circle);

	}

	void update()
	{
		for (int i = 0; i < bub.size(); i++)
		{
			bub.at(i)->update();
			bub.at(i)->render();
		}
	}


};

