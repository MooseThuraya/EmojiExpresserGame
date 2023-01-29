

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include <vector>
#include "time.h"													// for animation

bool animate = true;

const int numberOfVertices = 500;

// shader program

GLuint vBuffer = 0; 
GLuint vBuffer1 = 0;// GPU vertex buffer ID, valid if > 0
GLuint program = 0;
GLuint program1 = 0;// GLSL program ID, valid if > 0
//in vec3 color;
float scale = .5;													// make global
time_t startTime = clock();
const char *vertexShader = R"(
	#version 130
	in vec2 point;
	uniform float scale = 1;	
	out vec4 vColor;
	void main() {
		gl_Position = vec4(0.1*point, 0, 1);
		vColor = vec4(1, 1, 0, 0);
	}
)";

const char *pixelShader = R"(
	#version 130
	in vec4 vColor;
	out vec4 pColor;
	void main() {
		pColor = vColor;
	}
)";

// define circle composed of triangles, each point has an associated color

std::vector<vec2> points;
std::vector<vec3> colors;
std::vector<int3> triangles;
float pts[][2] = { {-1,-1}, {-1,1}, {1,1}, {1,-1} };
vec3 HSVtoRGB(vec3 hsv) {
	hsv.x = fmod(360.f*hsv.x, 360.f);
	if (abs(hsv.y) < FLT_MIN) return vec3(hsv.z, hsv.z, hsv.z);
	if (abs(hsv.x-360.f) < FLT_MIN) hsv.x = 0;
	hsv.x /= 60.f;
	int i = (int) floor(hsv.x);
	float f = hsv.x-i, p = hsv.z*(1-hsv.y), q = hsv.z*(1-(hsv.y*f)), t = hsv.z*(1-(hsv.y*(1-f)));
	if (i == 0) return vec3(hsv.z, t, p);
	if (i == 1) return vec3(q, hsv.z, p);
	if (i == 2) return vec3(p, hsv.z, t);
	if (i == 3) return vec3(p, q, hsv.z);
	if (i == 4) return vec3(t, p, hsv.z);
	return vec3(hsv.z, p, q);
}

void InitCircleBuffer(int nCircumPoints) {
	// total #points = #points along circumference, plus center
	points.resize(nCircumPoints+1);
	colors.resize(nCircumPoints+1);
	// center point
	points[0] = vec2(0, 0);
	colors[0] = vec3(1, 1, 0);
	// circumference points
	for (int i = 0; i < nCircumPoints; i++) {
		float t = (float) i / (nCircumPoints-1), a = 2*3.141592f*t;
		points[i+1] = vec2(cos(a), sin(a));
		//colors[i+1] = HSVtoRGB(vec3(t, 1, 0));
	}
	// triangles
	triangles.resize(nCircumPoints);
	for (int i = 0; i < nCircumPoints; i++)
		triangles[i] = int3(0, i+1, i == nCircumPoints-1? 1 : i+2);
	for (int i = 0; i < (int) triangles.size(); i++)
		int3 t = triangles[i];
	// create GPU buffer, make it active, allocate memory and copy vertices
	
	glGenBuffers(2, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

	// allocate and fill vertex buffer
	int vsize = sizeof(vec2)*points.size(), csize = sizeof(vec3)*colors.size();
	glBufferData(GL_ARRAY_BUFFER, vsize+csize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, points.data());
	glBufferSubData(GL_ARRAY_BUFFER, vsize, csize, colors.data());
	glGenBuffers(1, &vBuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer1);
	float pts[][2] = { {-1,-1}, {-1,1}, {1,1}, {1,-1} };
	glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);
	
}
// display
void MouseWheel(GLFWwindow* window, double ignore, double spin) {
	scale *= spin > 0 ? 1.1f : .9f;
}


void Display() {
	if (animate) {
		float dt = (float)(clock() - startTime) / CLOCKS_PER_SEC;		// elapsed time
		float freq = .5f, ang = 2 * 3.1415f * dt * freq;					// current angle
		scale = (1 + sin(ang)) / 2;
	}
	SetUniform(program, "scale", scale);
	// clear screen to grey
	glClearColor(.5f, .5f, .5f, 1);
	//glColor4f(1.0f, 1.0f, 0.0f, 0.0f); yellow

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	int vsize = sizeof(vec2)*points.size();
	VertexAttribPointer(program, "point", 2, 0, (void *) 0);
	//VertexAttribPointer(program, "points", 3, 0, (void *) vsize);
	VertexAttribPointer(program1, "point", 2, 0, (void*)0);
	glDrawElements(GL_TRIANGLES, 3*triangles.size(), GL_UNSIGNED_INT, triangles.data());
	glDrawArrays(GL_QUADS, 0, 4);
	glFlush();
}

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

// application

void Close() {
	// unbind vertex buffer and free GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vBuffer >= 0)
		glDeleteBuffers(1, &vBuffer);
}

int main() {
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow *window = glfwCreateWindow(600, 600, "Emoji-Test-2", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 1;
	}
	glfwSetWindowPos(window, 200, 200);
	glfwSetWindowSizeCallback(window, Resize);
	glfwMakeContextCurrent(window);
	glfwSetScrollCallback(window, MouseWheel);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
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

