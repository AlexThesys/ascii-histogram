#include "Window.h"

Window::Window(GLint w, GLint h) : width(w), height(h) {
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int Window::initialise() {
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _APPLE_
	glfwWindowHInt(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _APPLE_


    mainWindow = glfwCreateWindow(width, height, "openglTest5", nullptr, nullptr);
    if (mainWindow == nullptr) {
		puts("Failed to create GLFW widnow! Exiting...");
		glfwTerminate();
        return -1;
	}

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwSetFramebufferSizeCallback(mainWindow, glfw_onFrameBufferSize);

	glfwMakeContextCurrent(mainWindow);

	createCallbacks();

	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(mainWindow, double(bufferWidth) / 2.0, double(bufferHeight) / 2.0);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		printf("glew initialization error: %d\n", glewError);
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
        return -1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
	Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

	//close the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			theWindow->keys[key] = true;
		else if (action == GLFW_RELEASE)
			theWindow->keys[key] = false;
	}
}

void Window::handleMouse(GLFWwindow *window, double xPos, double yPos) {
	Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
	//current - last coordinates
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;  //non-inverted controls
	//current = last coordinates
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

void Window::handleScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

	theWindow->zoomChange = yoffset *2.0f;
}

void Window::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
	glfwSetScrollCallback(mainWindow, handleScroll);
}

void Window::glfw_onFrameBufferSize(GLFWwindow * window, int width, int height)
{
    Window *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
    theWindow->bufferWidth = width;
    theWindow->bufferHeight = height;
    glViewport(0, 0, width, height);
}

