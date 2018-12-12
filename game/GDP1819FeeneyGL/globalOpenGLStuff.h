#ifndef _globalOpenGLStuff_HG_
#define _globalOpenGLStuff_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Error/COpenGLError.h"
#include "cJoystickManager.h"

// GFLW keyboard callback
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessAsyncKeys(GLFWwindow* window);
void ProcessAsyncJoysticks(GLFWwindow* window, cJoystickManager* pJoysticks);

void cursor_position_callback( GLFWwindow* window, double xpos, double ypos );
void ProcessAsyncMouse(GLFWwindow* window);

void cursor_enter_callback(GLFWwindow* window, int entered);
extern bool g_MouseIsInsideWindow;	// 

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void joystick_callback(int joy, int event);

#endif

