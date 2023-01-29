// PointMeasure: measure points on image

#include <glad.h>
#include <glfw3.h>
#include <string.h>
#include <cstdio>
#include <vector>
#include "Draw.h"
#include "Sprite.h"

using namespace std;

// Image
Sprite texture;
string dir = "C:/Users/mosta/Graphics/Apps/";
string image = "valentine.PNG.tga";

// App Window, Interaction
int winWidth = 700, winHeight = 800;
int selected = -1;

// Outline

vector<vec2> outline;

void PrintOutline() {
	char buf[1000] = { 0 };
	printf("type filename: ");
	fgets(buf, 100, stdin);
	if (strlen(buf))
		buf[strlen(buf)-1] = 0; // remove <CR>
	string filename = dir+string(buf);
	FILE *file = fopen(filename.c_str(), "w");
	if (file) {
		fprintf(file, "vec2 outline[] = {\n");
		int npoints = outline.size();
		for (int i = 0; i < npoints; i++)
			fprintf(file, " {%3.2f, %3.2f}%s\n", outline[i].x, outline[i].y, i < npoints-1? "," : "");
		fprintf(file, "};\n");
		fclose(file);
		printf("printed = %s\n", filename.c_str());
	}
}

// Display

void Display(GLFWwindow* w) {
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	texture.Display();
	UseDrawShader();
	for (int i = 1; i < (int) outline.size(); i++)
		Line(outline[i-1], outline[i], 1, vec3(1, 1, 0), 1.5f);
	for (int i = 0; i < (int) outline.size(); i++)
		Disk(outline[i], 12, i == selected? vec3(0, 1, 0) : vec3(1, 0, 0));
	glFlush();
}

// Mouse

void MouseButton(GLFWwindow* w, int butn, int action, int mods) {
	if (action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(w, &x, &y);
		vec2 pScreen(x, y), pNDC(2*x/winWidth-1, 1-2*y/winHeight);
		selected = -1;
		for (int i = 0; i < (int) outline.size(); i++) {
			vec2 o = outline[i];
			vec2 oScreen((o.x+1)*.5f*(float)winWidth, (1-o.y)*.5f*(float)winHeight);
			if (length(pScreen-oScreen) < 12)
				selected = i;
		}
		if (selected == -1) {
			selected = outline.size();
			outline.push_back(pNDC);
		}
	}
}

void MouseMove(GLFWwindow* w, double x, double y) {
	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (selected >= 0)
			outline[selected] = vec2(2*x/winWidth-1, 1-2*y/winHeight);
	}
}

// Application

void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		switch(key) {
			case 'P': PrintOutline(); break;
			case 'R': outline.resize(0); break;
		}
}

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, winWidth = width, winHeight = height);
}

const char *usage = R"(Options:
	P: print
	R: reset points
)";

int main() {
	if (!glfwInit())
		return 1;
	GLFWwindow *w = glfwCreateWindow(winWidth, winHeight, "Point Measure", NULL, NULL);
	if (!w)
		return 1;
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	texture.Initialize(dir+image);
	glfwSetMouseButtonCallback(w, MouseButton);
	glfwSetCursorPosCallback(w, MouseMove);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	glfwSwapInterval(1);
	printf(usage);
	while (!glfwWindowShouldClose(w)) {
		Display(w);
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwDestroyWindow(w);
	glfwTerminate();
}
