#pragma once

#include <cstdio>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Window {
private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	bool keys[1024];  //ASCI keys
	GLfloat lastX, lastY, xChange, yChange, zoomChange;  //mouse coordinates
	static void handleMouse(GLFWwindow *window, double xPos, double yPos);
	static void handleScroll(GLFWwindow* window, double xoffset, double yoffset);
    static void glfw_onFrameBufferSize(GLFWwindow * window, int width, int height);
public:
	
	Window(GLint w = 1024, GLint h = 1024);
	~Window();
	int initialise();
	GLFWwindow *getWindow() { return mainWindow; }
	bool getShouldClose(){ return glfwWindowShouldClose(mainWindow); }
	void swapBuffers(){ glfwSwapBuffers(mainWindow); }
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool *getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getZoomChange();
};

inline Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}


inline GLfloat Window::getXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

inline GLfloat Window::getYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

inline GLfloat Window::getZoomChange() {
	GLfloat theChange = zoomChange;
	zoomChange = 0.0f;
	return theChange;
}
