#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

class Shader
{
public:
	const GLchar* codePtr;
	std::string shaderText;
	GLuint shaderID;
	Shader()
	{

	}
	//get the shader code from a text file
	Shader(const char* fileName)
	{
		std::ifstream inFile(fileName);
		while (inFile.good())
		{
			std::string line;
			std::getline(inFile, line);
			shaderText.append(line + "\n");
		}
		codePtr = shaderText.c_str();
		std::cout << "import success:" << std::endl;

	}

	void shaderFileName(const char* fileName)
	{
		std::ifstream inFile(fileName);
		while (inFile.good())
		{
			std::string line;
			std::getline(inFile, line);
			shaderText.append(line + "\n");
		}
		codePtr = shaderText.c_str();
		std::cout << "import success:" << std::endl;
	}
	
	void getShader(GLint type)
	{
		switch (type)
		{
		case 1:
			shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;
		case 2:
			shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			break;
		}
		
		glShaderSource(shaderID, 1, &codePtr, NULL);
		glCompileShader(shaderID);

		//debug shader compile
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		std::cout << "Shader compile: " << shaderID << " " << success << std::endl;

		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

	}
};