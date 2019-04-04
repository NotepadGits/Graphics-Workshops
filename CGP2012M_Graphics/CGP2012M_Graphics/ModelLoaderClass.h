#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class ModelImport
{
public:

	//Load .obj file
	//Inspiration from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ

	bool LoadOBJ2(const char* filename, std::vector<glm::vec3>&vertices, std::vector<glm::vec2>&texCoords, std::vector<glm::vec3>&normals, std::vector<GLuint>&indices)
	{
		//open the file for reading
		std::ifstream in(filename, std::ios::in);
		if (!in)
		{
			std::cout << ".obj file cannot be opened " << filename << std::endl;
			exit(1);
		}

		//read the file checking for .obj elements
		std::string line;
		while (getline(in, line)) {

			//read the vertex data first
			//OBJ files: vertices preceded by 'v'
			//OBJ files: texture coords preceded with 'vt'
			//OBJ files: precomputed normals preceded with 'vn'
			//OBJ files: faces (index of vertices/texCoords/normals)
			if (line.substr(0, 2) == "v ")
			{
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				vertices.push_back(v);
			}

			if (line.substr(0, 3) == "vt ")
			{
				std::istringstream s(line.substr(2));
				glm::vec2 t;
				s >> t.x; s >> t.y;
				texCoords.push_back(t);
			}

			if (line.substr(0, 3) == "vn ")
			{
				std::istringstream s(line.substr(2));
				glm::vec3 n;
				s >> n.x; s >> n.y; s >> n.z;
				normals.push_back(n);
			}

			if (line.substr(0, 2) == "f ")
			{
				std::istringstream s(line.substr(2));
				//vertex/texture coord/normal x 3
				GLuint v1 = 0, t1 = 0, n1 = 0;
				GLuint v2 = 0, t2 = 0, n2 = 0;
				GLuint v3 = 0, t3 = 0, n3 = 0;
				char temp;
				s >> v1;
				s >> temp;
				s >> t1;
				s >> temp;
				s >> n1;

				s >> v2;
				s >> temp;
				s >> t2;
				s >> temp;
				s >> n2;

				s >> v3;
				s >> temp;
				s >> t3;
				s >> temp;
				s >> n3;
				//opengl numbering is '0' based
				//.obj format uses 1 
				//we need to reduce the indices by 1 before use by opengl
				v1--; t1--; n1--;
				v2--; t2--; n2--;
				v3--; t3--; n3--;
				//first face
				indices.push_back(v1); indices.push_back(t1); indices.push_back(n1);
				//second face
				indices.push_back(v2); indices.push_back(t2); indices.push_back(n2);
				//third face
				indices.push_back(v3); indices.push_back(t3); indices.push_back(n3);
			}
		}//end of while
		std::cout << "Model: " << *filename << " loaded" << std::endl;
	}//end of LoadOBJ
};