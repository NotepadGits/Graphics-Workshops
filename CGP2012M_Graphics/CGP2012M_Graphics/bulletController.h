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
		if (bul.size() < 1)
		{
			bul.push_back(new bullet(plr));
		}
	}

	void update()
	{
		for (int I = 0; I < bul.size(); I++)
		{

			if (bul.at(I)->position.x > 4 || bul.at(I)->position.x < 0 || bul.at(I)->position.y > 3 || bul.at(I)->position.y < 0)
			{

				bul.erase(bul.begin() + I);
				return;
			}


			for (int i = 0; i < spwn->bub.size(); i++)
			{





				glm::vec3 bulPosition = bul[I]->position;
				glm::vec3 bubPosition = spwn->bub.at(i)->position;

				float dist = glm::distance(bulPosition, bubPosition);

				/*if (bulPosition.x >= spwn->bub.at(i)->position.x - 0.02f && bulPosition.x <= spwn->bub.at(i)->position.x + 0.02f && bulPosition.y >= spwn->bub.at(i)->position.y - 0.02f && bulPosition.y <= spwn->bub.at(i)->position.y + 0.02f)*/

				if(dist < spwn->bub.at(i)->radius)
				{

					//std::cout << bul[I]->position.x << std::endl;

					//std::cout << "aaaaaaaaaaa" << std::endl;
					//bul[I]->position.x = 100;

					bul.erase(bul.begin() + I);
					spwn->bub.erase(spwn->bub.begin() + i);
					return;
				}

		



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

