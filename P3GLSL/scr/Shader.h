#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>
#include "auxiliar.h"

#include <string>
#include <GL/glew.h>

class Shader
{
private:
	unsigned int v;
	unsigned int f;

	GLuint loadShader(const char* fileName, GLenum type) {
		unsigned int fileLen;
		char* source = loadStringFromFile(fileName, fileLen);
		//////////////////////////////////////////////
		//Creación y compilación del Shader
		GLuint shader;
		shader = glCreateShader(type);
		glShaderSource(shader, 1,
			(const GLchar**)&source, (const GLint*)&fileLen);
		glCompileShader(shader);

		delete[] source;

		//Comprobamos que se compiló bien
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			//Calculamos una cadena de error
			GLint logLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
			char* logString = new char[logLen];
			glGetShaderInfoLog(shader, logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete[] logString;
			glDeleteShader(shader);
			exit(-1);
		}




		return shader;
	}
	
public:
    //  ID del programa
    unsigned int ID;

	Shader() {};
    void init(const char* vname, const char* fname) {
		v = loadShader(vname, GL_VERTEX_SHADER);
		f = loadShader(fname, GL_FRAGMENT_SHADER);
		
		ID = glCreateProgram();
		glAttachShader(ID, v);
		glAttachShader(ID, f);

		glBindAttribLocation(ID, 0, "inPos");
		glBindAttribLocation(ID, 1, "inColor");
		glBindAttribLocation(ID, 2, "inNormal");
		glBindAttribLocation(ID, 3, "inTexCoord");
		glBindAttribLocation(ID, 4, "inTangent");

		glLinkProgram(ID);

		int linked;
		glGetProgramiv(ID, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			//Calculamos una cadena de error
			GLint logLen;
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLen);
			char* logString = new char[logLen];
			glGetProgramInfoLog(ID, logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete[] logString;
			glDeleteProgram(ID);
			ID = 0;
			exit(-1);
		}

		
    }
	
	void use() {
		glUseProgram(ID);
	}
    
	//AUXILIARES

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str() ), 1, GL_FALSE,
			&(value[0][0]));
	}
	void setVec3(const std::string& name, glm::vec3 value) const {
		
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void destroy() {
		glDetachShader(ID, v);
		glDetachShader(ID, f);
		glDeleteShader(v);
		glDeleteShader(f);
		glDeleteProgram(ID);
	}

};