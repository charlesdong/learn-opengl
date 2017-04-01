#include "shader.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ifstream;

const int PROGRAM = 0;

GLuint loadShader(const char * filename, GLuint type);
GLuint makeProgram(GLuint vShader, GLuint fShader);
const char * readFile(const char * filename);
bool checkErrors(GLuint id, GLuint type);
void outputInfoLog(GLuint id, GLuint type);
const char * getName(GLuint type);

GLuint loadProgram(const char * vFilename, const char * fFilename)
{
	GLuint vShader = loadShader(vFilename, GL_VERTEX_SHADER);
	GLuint fShader = loadShader(fFilename, GL_FRAGMENT_SHADER);
	GLuint program = makeProgram(vShader, fShader);
	return program;
}

GLuint loadShader(const char * filename, GLuint type)
{
	const char * source;
	GLuint shader;

	// read the shader's source code from file
	source = readFile(filename);
	// if failed, return 0 as failure
	if (source == nullptr)
		return 0;

	// build and compile the shader
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	// if errors occur, uninitialize and return 0 as failure
	if (!checkErrors(shader, type))
	{
		outputInfoLog(shader, type);
		glDeleteShader(type);
		return 0;
	}

	delete[] source;
	return shader;
}

GLuint makeProgram(GLuint vShader, GLuint fShader)
{
	// if any one of the vertex and fragment shader failed to be loaded, return 0 as failure
	if (vShader == 0 || fShader == 0)
	{
		cout << "Failed to create shader program!\n";
		return 0;
	}

	// build and link the program
	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	// if errors occur, uninitialize and return 0 as failure
	if (!checkErrors(program, PROGRAM))
	{
		outputInfoLog(program, PROGRAM);
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		glDeleteProgram(program);
		return 0;
	}

	// delete the shaders (they are no longer used)
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return program;
}

const char * readFile(const char * filename)
{
	ifstream fin;
	char * source;
	int size = 0;

	// open the shader file
	fin.open(filename);
	if (!fin.is_open())
	{
		cout << "Failed to open file " << filename << " (maybe not exist)!\n";
		return nullptr;
	}
	// allocate enough memory to store the shader source
	fin.seekg(0, std::ios_base::end);
	source = new char[(int)fin.tellg() + 1];
	// read the source code
	fin.seekg(0, std::ios_base::beg);
	while (!fin.eof())
	{
		source[size] = fin.get();
		size++;
	}
	source[size] = '\0';

	return source;
}

bool checkErrors(GLuint id, GLuint type)
{
	GLint status;
	if (type != PROGRAM)
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	else
		glGetProgramiv(id, GL_LINK_STATUS, &status);
	return status == GL_TRUE;
}

void outputInfoLog(GLuint id, GLuint type)
{
	char * infoLog;
	int size;
	if (type != PROGRAM)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);
		infoLog = new char[size + 1];
		glGetShaderInfoLog(id, size + 1, nullptr, infoLog);
		cout << "Failed to compile shader, type: " << getName(type) << ", info log:\n" << infoLog << endl;
		delete[] infoLog;
	}
	else
	{
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);
		infoLog = new char[size + 1];
		glGetProgramInfoLog(id, size + 1, nullptr, infoLog);
		cout << "Failed to link program, info log:\n" << infoLog << endl;
		delete[] infoLog;
	}
}

const char * getName(GLuint type)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		return "vertex";
	case GL_FRAGMENT_SHADER:
		return "fragment";
	default:
		return "unknown";
	}
}