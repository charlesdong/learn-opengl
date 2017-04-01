#include <iostream>
using std::cout;
using std::endl;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"

int main()
{
	if (!glfwInit())
	{
		cout << "Failed to init GLFW!\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow * window = glfwCreateWindow(800, 600, "First triangle", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create window using GLFW!\n";
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		cout << "Failed to init GLEW!\n";
		return -1;
	}

	// load the vertex and fragment shader from file and creates the program
	GLuint program = loadProgram("shader.vert", "shader.frag");
	if (program == 0)
		return -1;

	// vertex data
	GLfloat vertexes[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.5f
	};
	// VAO and VBO
	GLuint vao, vbo;

	// generate objects
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// bind the VAO
	glBindVertexArray(vao);

	// bind the VBO and attach the vertex data to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

	// set the vertex attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// unbind the objects to save the settings
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT);

		// activate the shader program
		glUseProgram(program);

		// bind the VAO and draw the triangle
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete the objects
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}