/*
List of Demo to try out
FIRE_WAVE,
SPIRAL,
FIRE_SPARKS,
FIRE,
FIRE_FAN,
FALLING_SQUARE,
BOKEH,
FLOWER_TRIANGLE,
FIRE_FLY


Instruction
1. You can pick a demo and run it but can never edit its default properties.
2. So if you click run again, the property panel will revert back to the default properties
3. You can edit any of the particle properties in the property panel but it will not be saved;
4. The setup in the property panel will reset into demo data if demo is clicked to run.

*/


#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "AllHeader.h"
#include "Application.h"
#include "Vector3.h"
#include "bitmap.h"
#include "Color.h"
#include "ColorDeclaration.h"
#include "ParticleAffectorDeclaration.h"
#include "GUI.h"

const int RESOLUTION_X = 1280;
const int RESOLUTION_Y = 720;

void Stealth()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0.0f, 0.0f, width, height);				// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0.0f, width, 0.0f, height, -1000.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

int main(void)
{
	Stealth();
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "Particle System by Teena 0118856", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwMakeContextCurrent(window);
	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glfwSwapInterval(1); //enable/disable vsync

	Application app(window, RESOLUTION_X, RESOLUTION_Y);
	app.Start();

	GUI gui(window, RESOLUTION_X, RESOLUTION_Y);
	gui.SetCurrentApplication(&app);

	float deltaTime = 0.0f;
	int width;
	int height;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		/* Render here */
		glfwGetFramebufferSize(window, &width, &height);
		app.SetWindowSize(width, height);
		gui.SetWindowSize(width, height);
		gui.SetCurrentParticleSystem(app.curPS);

		if (app.play == true)
		{
			app.UpdateInput();
			app.Update(deltaTime);
		}
		app.Draw();
		gui.DrawGUI();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}