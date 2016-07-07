#include "Application.h"

int Application::width = 600;
int Application::height = 400;

bool Application::mouse1active = false;
bool Application::mouse2active = false;

Application* Application::application = new Application();



Application::Application(){

}


Application::~Application()
{
	glfwTerminate();

}

bool Application::init()
{
	if (!initWindow())
		return false;

	glfwSetKeyCallback(window, key_callback);

	initSecondMouse();

	return true;
}

bool Application::initSecondMouse()
{

	device[0].usUsagePage = 1;
	device[0].usUsage = 2;
	device[0].dwFlags = 0;
	device[0].hwndTarget = NULL;

	if (RegisterRawInputDevices(device, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
		std::cout << "Error registering mouse" << std::endl;

	return true;
}

bool Application::initWindow()
{
	if (!glfwInit())
	{
		std::cout << "GLFW Init failed!" << std::endl;
		return false;
	}

	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);


	window = glfwCreateWindow(width, height, "Window", (fullscreen) ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	handle = glfwGetWin32Window(window);

	if (!window) {
		std::cout << "Creating Window failed!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(window);

	//store the current message event handler for the window
	currentWndProc = (WNDPROC)GetWindowLongPtr(handle, GWL_WNDPROC);
	SetWindowLongPtr(handle, GWL_WNDPROC, (long)winProc);

	glClearColor(0.49f, 0.71f, 1.0f, 0.0f);

	glViewport(0, 0, width, height);

	glfwSetCursorPos(window, width / 2, height / 2);

	return true;
}


void Application::run() {

	// Game Loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSetWindowTitle(window, "Multiple Mice Example");
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void Application::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		
		default:
			break;
		}
	}
}

void Application::OnRawInput(bool inForeground, HRAWINPUT hRawInput)
{
	UINT dataSize;
	GetRawInputData(hRawInput, RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

	if (dataSize == 0)
		return;
	if (dataSize > m_RawInputMessageData.size())
		m_RawInputMessageData.resize(dataSize);

	void* dataBuf = &m_RawInputMessageData[0];
	GetRawInputData(hRawInput, RID_INPUT, dataBuf, &dataSize, sizeof(RAWINPUTHEADER));

	const RAWINPUT *raw = (const RAWINPUT*)dataBuf;

	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		HANDLE deviceHandle = raw->header.hDevice;

		const RAWMOUSE& mouseData = raw->data.mouse;

		USHORT flags = mouseData.usButtonFlags;
		short wheelDelta = (short)mouseData.usButtonData;
		LONG x = mouseData.lLastX, y = mouseData.lLastY;
		
		/*------------------------------------
		* This prints out all the incoming 
		* mouse data. The commented block 
		* below shows, how you could use that 
		* data. 
		*------------------------------------*/

		wprintf(L"Mouse: Device=0x%08X, Flags=%04x, WheelDelta=%d, X=%d, Y=%d\n",deviceHandle, flags, wheelDelta, x, y);

		/*
		//Safe handles 
		if (mouse1 == 0)
		{
			mouse1 = deviceHandle;
		}
		else if (deviceHandle != mouse1 && mouse2 == 0)
		{
			mouse2 = deviceHandle;
		}

		//Extract mouse information
		if (deviceHandle == mouse1)
		{
			mouse1active = true;
			mouse2active = false;

			if (flags == 1)
				std::cout << "Mouse1 right button clicked" << std::endl;
			if (flags == 2)
				std::cout << "Mouse1 right button released" << std::endl;
			if (flags == 4)
				std::cout << "Mouse1 left button clicked" << std::endl;
			if (flags == 8)
				std::cout << "Mouse1 left button released" << std::endl;
	
		}
		if (deviceHandle == mouse2)
		{
			mouse1active = false;
			mouse2active = true;

			if (flags == 1)
				std::cout << "Mouse2 right button clicked" << std::endl;
			if (flags == 2)
				std::cout << "Mouse2 right button released" << std::endl;
			if (flags == 4)
				std::cout << "Mouse2 left button clicked" << std::endl;
			if (flags == 8)
				std::cout << "Mouse2 left button released" << std::endl;
		}*/

	}
}

LRESULT CALLBACK Application::winProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam){

	switch (Msg)
	{
	case WM_INPUT:
		application->OnRawInput(GET_RAWINPUT_CODE_WPARAM(wParam) == RIM_INPUT, (HRAWINPUT)lParam);
		break;

	case WM_CLOSE:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
		break;

	default:
		return CallWindowProc(currentWndProc, handle, Msg, wParam, lParam);
		break;
	}
	return 0;
}