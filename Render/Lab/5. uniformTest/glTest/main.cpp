#include "MyGlWindow.h"

MyGlWindow* win;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void win_size_callback(GLFWwindow* window, int width, int height)
{
	win->setSize(width, height);
}

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "failed to initialize glfw\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800, height = 800;
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Framework", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (gl3wInit())
	{
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}
	if (!gl3wIsSupported(3, 2))
	{
		fprintf(stderr, "OpenGL 3.2 not supported\n");
		return -1;
	}
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	glfwSwapInterval(1); //vsync

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowTitle(window, u8"나의 윈도우");

	win = new MyGlWindow(width, height);
	glfwSetWindowSizeCallback(window, win_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		win->draw();

		//swap front & back buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}