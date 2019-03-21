#pragma once

//standard libraries and stuff
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"

//The windows library
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"

//OpenGL maths library
#define GLM_FORCE_RADIANS // force glm to use radians
//----
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>






//Include the textured triangle class
#include "Triangle_T.h"
#include "Square.h"
#include "Circle.h"

#include "Events.h"
#include "Camera.h"

#include "spawner.h"
#include "bullet.h"
#include "bulletController.h"
#include "bomb.h"
#include "Bombtext.h"
#include "Lives.h"