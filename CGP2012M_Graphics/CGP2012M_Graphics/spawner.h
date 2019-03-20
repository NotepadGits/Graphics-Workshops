#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"
#include "Camera.h"
#include <vector>
#include "Circle.h"


class Spawner
{
public:
	std::vector<Circle*> bub;
	float rVal;
	unsigned int x=0;
	float radius;




	Spawner()
	{
		Circle cr(0.2f);


		//bub.push_back(&cr);

		srand(time(0));
	}

	void spawn()
	{

		//(float)radius = rand() % 3 / 10.0f;
		radius = 0.2;

		rVal = rand() % 333;
		if (rVal == 1)
		{

			bub.push_back(new Circle(radius));
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

