#ifndef _globalOpenGLStuff_HG_
#define _globalOpenGLStuff_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Error/COpenGLError.h"

// GFLW keyboard callback
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessAsynKeys(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#endif

