#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <string>
#include "windows.h"

class GLerror
{
public:
	//debug callback function
	//Inspired by blog.nobel-jorgensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
	//
	static void APIENTRY openglCallbackFunction(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) 
	{
		std::cout << "Value: " << *(int*)userParam << std::endl;

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			std::cout << "ERROR" << std::endl;
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			std::cout << "DEPRECATED_BEHAVIOR" << std::endl;
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			std::cout << "UNDEFINED_BEHAVIOR" << std::endl;
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			std::cout << "PORTABILITY" << std::endl;
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			std::cout << "PERFORMANCE" << std::endl;
			break;
		case GL_DEBUG_TYPE_OTHER:
			std::cout << "OTHER" << std::endl;
			break;
		}

		//void pointer needs to be cast to its original type
		//if you don't know the original type, this can't be done
		std::cout << "*********************************" << std::endl;
		std::cout << "ERROR CALLBACK at location = " << *(int*)userParam << std::endl;
		std::cout << "ERROR CALLBACK: " << getStringForSource(source).c_str() << std::endl;
		std::cout << "ERROR CALLBACK: " << message << std::endl;
		if (severity == GL_DEBUG_SEVERITY_HIGH) {
			std::cout << "Aborting..." << std::endl;
			abort();
		}
	}

	static std::string getStringForSource(GLenum source)
	{
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return "Window system";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return "Shader compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return "Third party";
		case GL_DEBUG_SOURCE_APPLICATION:
			return "Application";
		case GL_DEBUG_SOURCE_OTHER:
			return "Other";
		default:
			//assert(false);
			return "";
		}
	}
};