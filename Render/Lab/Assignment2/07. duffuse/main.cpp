#include "MyGlWindow.h"

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;

MyGlWindow* win;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    win->setSize(width, height);
    win->setAspect(width / (float)height);

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    cx = xpos;
    cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (GLFW_PRESS == action)
            lbutton_down = true;
        else if (GLFW_RELEASE == action)
            lbutton_down = false;
    }

    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (GLFW_PRESS == action)
            rbutton_down = true;
        else if (GLFW_RELEASE == action)
            rbutton_down = false;
    }

    else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (GLFW_PRESS == action)
            mbutton_down = true;
        else if (GLFW_RELEASE == action)
            mbutton_down = false;
    }
}


void mouseDragging(double width, double height)
{
    if (lbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->m_viewer->rotate(fractionChangeX, fractionChangeY);
    }
    else if (mbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->m_viewer->zoom(fractionChangeY);
    }
    else if (rbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->m_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
        printf("1");
    }
    m_lastMouseX = cx;
    m_lastMouseY = cy;
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

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetKeyCallback(window, key_callback);

	win = new MyGlWindow(width, height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		win->draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        mouseDragging(width, height);
    }

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}