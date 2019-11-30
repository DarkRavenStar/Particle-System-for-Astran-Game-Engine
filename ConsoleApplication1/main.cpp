#include <iostream>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "Application.h"
#include "GL/glu.h"
#include "Vector3.h"
//#include "bitmap.h"



const int RESOLUTION_X = 640;
const int RESOLUTION_Y = 480;

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0.0f, 0.0f, width, height);	// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);
    glfwMakeContextCurrent(window);
	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);


	glfwSwapInterval(0); //enable/disable vsync

	Application app;
	app.start();
	float deltaTime;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
	    if(glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

	    /* Render here */
	    app.update(deltaTime);
	    app.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
