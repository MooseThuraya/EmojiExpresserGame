#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include <vector>

//**Commented animation code lines temporarily
//bool animate = true;
const int numberOfVertices = 100; 

// shader program
//float scale = .5; // make global
//time_t startTime = clock();

//**Added 2 buffers for circle and eye
GLuint vBuffer = 0; // GPU vertex buffer ID, valid if > 0
GLuint program = 0; // GLSL program ID, valid if > 0

GLuint vBufferEye = 0; // GPU vertex buffer ID, valid if > 0
GLuint programEye = 0; // GLSL program ID, valid if > 0


const char* vertexShader = R"(
	#version 130
	in vec2 point;
	uniform float scale = 1;
	void main() {
		gl_Position = vec4(point, 0, 1);
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

//**defined a circle for both face and eye
std::vector<vec2> points;
std::vector<int3> triangles;
std::vector<vec2> pointsEye;
std::vector<int3> trianglesEye;

//** Utility Function to create face circle 
void InitCircleBuffer(int nCircumPoints) {
	points.resize(nCircumPoints+1);
	points[0] = vec2(0, 0);
	// circumference points
	for (int i = 0; i < nCircumPoints; i++) {
		float t = (float) i / (nCircumPoints-1), a = 2*3.141592f*t;
		points[i+1] = vec2(sin(a), cos(a));
	}

	// triangles
	triangles.resize(nCircumPoints);
	for (int i = 0; i < nCircumPoints; i++)
		triangles[i] = int3(0, i+1, i == nCircumPoints-1? 1 : i+2);
	// create GPU buffer for face circle
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	
	int vsize = sizeof(vec2) * points.size();
	
	glBufferData(GL_ARRAY_BUFFER, vsize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vsize,points.data()); 
}

void Display() {
	
	//SetUniform(program, "scale", scale);
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	// display emoji face
	SetUniform(program, "color", vec3(1, 1, 0));
	VertexAttribPointer(program, "point", 2, 0, (void *) 0);
	glDrawElements(GL_TRIANGLES, 3*triangles.size(), GL_UNSIGNED_INT, triangles.data());
	glFlush();
}

// application

void Close() {
	// unbind both vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vBuffer >= 0)
		glDeleteBuffers(1, &vBuffer);
}

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_SAMPLES, 4); 
	GLFWwindow *window = glfwCreateWindow(600, 600, "EmojiEyeFormation", NULL, NULL); 
	if (!window) {
		glfwTerminate();
		return 1;
	}
	glfwSetWindowPos(window, 200, 200);
	glfwSetWindowSizeCallback(window, Resize);
	glfwMakeContextCurrent(window);
	//glfwSetScrollCallback(window, MouseWheel);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	//** In if statement if program is used only white circle gets displayed, not knowing how to display both or switch 2 programs
	if (!(program = LinkProgramViaCode(&vertexShader, &pixelShader)))
		getchar();
		
	InitCircleBuffer(numberOfVertices);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		
		Display();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Close();
	glfwDestroyWindow(window);
	glfwTerminate();
}

