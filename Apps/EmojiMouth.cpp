// EmojiTwoCircles.cpp

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include <vector>

using std::vector;

// Circle
const int nCircleVertices = 100;
vector<int3> circleTriangles;

// OpenGL Identifiers
GLuint vCircleBuffer = 0;
GLuint program = 0;

// Shaders

const char* vertexShader = R"(
	#version 130
	in vec2 point;
	uniform mat4 m = mat4(1);
	void main() {
		gl_Position = m*vec4(point, 0, 1);
	}
)";

const char* pixelShader = R"(
	#version 130
	uniform vec3 color;
	out vec4 pColor;
	void main() {
		pColor = vec4(color, 1);
	}
)";

// Display

void DrawCircle(float x, float y, float scale, vec3 color) {
	mat4 m = Translate(x, y, 0) * Scale(scale, scale, 1);
	SetUniform(program, "m", m);
	SetUniform(program, "color", color);
	VertexAttribPointer(program, "point", 2, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, vCircleBuffer);
	glDrawElements(GL_TRIANGLES, 3 * circleTriangles.size(), GL_UNSIGNED_INT, circleTriangles.data());
}

void Display() {
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(program);
	// display yellow face and two red eyes
	DrawCircle(0, 0, 1, vec3(1, 1, 0));
	//DrawCircle(-.4f, .25f, .15f, vec3(0, 0, 0));
	//DrawCircle(.4f, .25f, .15f, vec3(0, 0, 0));
	DrawCircle(-0.f, -.5f, -.2f, vec3(0, 0, 0));
	glFlush();
}

// Circle Construction

GLuint InitCircle(int nCircumPoints) {
	// set vertices
	vector<vec2> points(nCircumPoints + 1);
	points[0] = vec2(0, 0);
	// circumferential points
	for (int i = 0; i < nCircumPoints; i++) {
		float t = (float)i / (nCircumPoints - 1), a = 1 * 3.141592f * t;
		points[i + 1] = vec2(cos(a), sin(a));
	}
	// triangles
	circleTriangles.resize(nCircumPoints);
	for (int i = 0; i < nCircumPoints; i++)
		circleTriangles[i] = int3(0, i + 1, i == nCircumPoints - 1 ? 1 : i + 2);
	// buffer to GPU
	GLuint vBuffer;
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * points.size(), points.data(), GL_STATIC_DRAW);
	return vBuffer;
}

// Application

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	// initialize window
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* w = glfwCreateWindow(600, 600, "Emoji", NULL, NULL);
	if (!w) {
		glfwTerminate();
		return 1;
	}
	glfwSetWindowPos(w, 200, 200);
	glfwSetWindowSizeCallback(w, Resize);
	// initialize OpenGL
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// init shader program and vertex buffer
	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	vCircleBuffer = InitCircle(nCircleVertices);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vCircleBuffer >= 0)
		glDeleteBuffers(1, &vCircleBuffer);
	glfwDestroyWindow(w);
	glfwTerminate();
}
