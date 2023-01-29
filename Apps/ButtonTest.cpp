// AudioTests.cpp

#include <glad.h>
#include <GLFW/glfw3.h>
#include "Draw.h"
#include "Misc.h"
#include "Text.h"
#include "Widgets.h"
#include <stdio.h>

int winW = 400, winH = 200;

Button b("Hello", 100, 120, 100, 30, vec3(1,1,0));
Toggler t("There", 100, 80, 15);

void Display(GLFWwindow *w) {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	UseDrawShader(ScreenMode());
	b.Draw();
	t.Draw();
	glFlush();
}

void MouseButton(GLFWwindow *w, int butn, int action, int mods) {
	double x, y;
	glfwGetCursorPos(w, &x, &y);
	y = winH-y;
	if (action == GLFW_PRESS) {
		if (b.Hit(x, y)) printf("Button HIT!\n");
		if (t.Hit(x, y)) printf("Toggle HIT!\n");
	}
}

void Resize(GLFWwindow *w, int width, int height) {
	glViewport(0, 0, winW = width, winH = height);
}

int main(int ac, char **av) {
	glfwInit();
	GLFWwindow *w = glfwCreateWindow(winW, winH, "Button Test", NULL, NULL);
	glfwSetWindowPos(w, 100, 100);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetMouseButtonCallback(w, MouseButton);
	while (!glfwWindowShouldClose(w)) {
		Display(w);
		glfwPollEvents();
		glfwSwapBuffers(w);
	}
	glfwDestroyWindow(w);
	glfwTerminate();
}
