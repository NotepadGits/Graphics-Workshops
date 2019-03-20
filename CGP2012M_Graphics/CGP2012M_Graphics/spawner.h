#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"
#include "Camera.h"
#include <vector>
#include "Circle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Spawner
{
public:
	std::vector<Circle*> bub;
	float rVal;
	unsigned int x=0;
	float radius = 0.02f;




	Spawner()
	{
		//Circle cr(0.2f);


		//bub.push_back(&cr);
		Circle* circle = new Circle(0.2f);

		float r = (rand() / ((float)RAND_MAX));
		r = 0.7 + r * (1.0 - 0.7);

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
			Circle* circle = new Circle(radius);

			float r = (rand() / ((float)RAND_MAX));
			r = 0.7 + r * (1.0 - 0.7);

			circle->scale = glm::scale(circle->scale, glm::vec3(r));

			bub.push_back(circle);
		}

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

