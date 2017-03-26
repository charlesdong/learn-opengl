#include <iostream>
using std::cout;
using std::endl;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
	// init GLFW
	if (!glfwInit())
	{
		cout << "Failed to init GLFW!\n";
		return -1;
	}

	// set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create window
	GLFWwindow * window = glfwCreateWindow(800, 600, "First window", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create window using GLFW!\n";
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init GLFW
	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		cout << "Failed to init GLEW!\n";
		return -1;
	}

	// set clear color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		// if Esc is pressed, set the close flag of the window
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		// clear the window
		glClear(GL_COLOR_BUFFER_BIT);

		// swap and poll
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// cleanup
	glfwTerminate();
	return 0;
}