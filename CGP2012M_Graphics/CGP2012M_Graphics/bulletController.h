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
#include "Triangle_T.h"
#include "bullet.h"
#include "spawner.h"

class Control
{
public:

	Triangle_T* plr;
	std::vector<bullet*> bul;
	Spawner* spwn;


	Control(Triangle_T* plr, Spawner* spwn)
	{
		this->spwn = spwn;
		this->plr = plr;
	};

	void shoot()
	{
		if (bul.size() >= 0)
		{
			bul.push_back(new bullet(plr));
		}
	}

	void update()
	{
		for (int I = 0; I < bul.size(); I++)
		{
			for (int i = 0; i < spwn->bub.size(); i++)
			{





			//	glm::vec3 bulPosition = bul[I]->position;
			//	glm::vec3 bubPosition = spwn->bub.at(i)->position;

			//	if (abs(bulPosition.x - bubPosition.x) < spwn->radius)
			//	{

			//		bul.erase(bul.begin(), bul.end());
			//		spwn->bub.erase(spwn->bub.begin() + i);
			//	}


			}
		}
		
		for (int i = 0; i < bul.size(); i++)
		{
			bul.at(i)->update();
		}
	}



	void redner()
	{
		for (int i = 0; i < bul.size(); i++)
		{
			bul.at(i)->render();
		}
	}


};

