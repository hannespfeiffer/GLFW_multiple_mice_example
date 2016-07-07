#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <windows.h>
#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <GL/wglew.h>

#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

static HWND handle = NULL;
static WNDPROC currentWndProc;
static std::vector<char> m_RawInputMessageData; // Buffer

class Application
{
public:

	Application();

	~Application();

	bool init();

	bool initSecondMouse();

	void run();

	static Application* application;

	GLFWwindow* window;

	static int width;

	static int height;


	//------ Mouse specific stuff ------

	//Callback function for Windows API events
	static LRESULT CALLBACK Application::winProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam); 
	
	//Gets called by winProc when input data comes in
	void OnRawInput(bool inForeground, HRAWINPUT hRawInput);
	
	//Holds the mice
	RAWINPUTDEVICE device[1];

	//Mice
	HANDLE mouse1 = 0;
	HANDLE mouse2 = 0;

	//Can be used to determine which mouse the data comes from
	static bool mouse1active;
	static bool mouse2active;

private:

	bool initWindow();

	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

	bool fullscreen;

};


