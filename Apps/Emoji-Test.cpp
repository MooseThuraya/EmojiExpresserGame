// *** JB COMMENTED
// 4-ColorfulLetter.cpp: draw multiple triangles to form a colorful letter
// *** JB: Always Rename Your File! like:
// Emoji-Test-1-Commented.cpp: colorful circle draw test

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include <vector>
// *** JB: unneeded #include <math.h>
// *** JB: unneeded #include <GL/glu.h>

bool animate = true;
const int numberOfVertices = 100;
const int numberOfVertices1 = 55;// *** JB: 500 seems way too high 500;

// shader program
float scale = .5;													// make global
time_t startTime = clock();
GLuint vBuffer = 0; // GPU vertex buffer ID, valid if > 0
GLuint program = 0; // GLSL program ID, valid if > 0

const char *vertexShader = R"(
	#version 130
	in vec2 point;
	//in vec3 color;
	in vec2 point_new;
	out vec4 vColor;
	uniform float scale = 1;
	out vec4 vColor1;
	void main() {
		gl_Position = vec4(scale*point, 0, 1);
		vColor = vec4(1, 1, 0, 0);
		vColor1 = vec4(1,0,1,0);
	}
)";

const char *pixelShader = R"(
	#version 130
	in vec4 vColor;
	out vec4 pColor;
	in vec4 vColor1;
	out vec4 pColor1;
	void main() {
		pColor = vColor;
		pColor1 = vColor1;
	}
)";

// *** JB: the following code leftover from ColorfulLetter - delete!
// the letter T: 8 vertices, 8 colors, triangulation
//float points[][2] = {{-.25f, -.75f}, {-.25f, .3f}, {-.75f, .3f}, {-.75f,  .75f},
//					 { .75f,  .75f}, { .75f, .3f}, { .25f, .3f}, { .25f, -.75f}};
//float colors[][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
//					 {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
//int triangles[][3] = {{0, 7, 1}, {7, 6, 1}, {1, 3, 2}, {1, 4, 3}, {1, 6, 4}, {6, 5, 4}}; // good
//int triangles[][3] = {{0, 7, 1}, {7, 6, 1}, {2, 4, 3}, {2, 5, 4}}; // bad

/* the letter B: 10 vertices, 10 colors, 9 triangles
s
float points[][2] = {{-.15f, .125f}, {-.5f, -.75f}, {-.5f,   .75f}, {.17f, .75f}, {.38f, .575f},
					 {.38f,   .35f}, {.23f, .125f}, {.5f,  -.125f}, {.5f,  -.5f}, {.25f, -.75f}};

float colors[][3] = {{1, 1, 1}, {1, 0, 0}, {.5f, 0, 0}, {1, 1, 0}, {.5f, 1, 0},
					 {0, 1, 0}, {0, 1, 1}, {0,   0, 1}, {1, 0, 1}, {.5f, 0, .5f}};

int triangles[][3] = {{0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5},
					  {0, 5, 6}, {0, 6, 7}, {0, 7, 8}, {0, 8, 9}, {0, 9, 1}}; */

// *** JB: define dynamic arrays for points, colors, and triangles:
// define a circle composed of triangles, each point has an associated color
std::vector<vec2> points;
std::vector<vec2> point_new;
std::vector<vec3> colors;
std::vector<int3> triangles;
std::vector<int3> new_triangle;

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
// *** JB: begin new code
	// total #points = #points along circumference, plus center
	points.resize(nCircumPoints+1);
	point_new.resize(nCircumPoints + 1);
	colors.resize(nCircumPoints+1);
	// center point
	points[0] = vec2(0, 0);
	point_new[0] = vec2(0, 0);
	colors[0] = vec3(1, 1, 1);
	// circumference points
	for (int i = 0; i < nCircumPoints; i++) {
		float t = (float) i / (nCircumPoints-1), a = 2*3.141592f*t;
		points[i+1] = vec2(sin(a), cos(a));
		//colors[i+1] = HSVtoRGB(vec3(t, 1, 1));
	}

	// triangles
	triangles.resize(nCircumPoints);
	for (int i = 0; i < nCircumPoints; i++)
		triangles[i] = int3(0, i+1, i == nCircumPoints-1? 1 : i+2);
	for (int i = 0; i < (int) triangles.size(); i++)
		int3 t = triangles[i];


// *** JB: end new code
	// create GPU buffer, make it active, allocate memory and copy vertices
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	// allocate and fill vertex buffer
	// *** JB int vsize = sizeof(points), csize = sizeof(colors);
	int vsize = sizeof(vec2)*points.size(), csize = sizeof(vec3)*colors.size();
	int vsize2 = sizeof(vec2) *point_new.size(), csize1 = sizeof(vec3)*colors.size();// *** JB computed differently for dynamic arrays
	
	glBufferData(GL_ARRAY_BUFFER, vsize+csize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, points.data()); // *** JB: note difference between static and dynamic arrays
	glBufferSubData(GL_ARRAY_BUFFER, vsize, csize, colors.data());
}

void InitEyesBuffer(int nCircumPoints) {
	// *** JB: begin new code
		// total #points = #points along circumference, plus center
	points.resize(nCircumPoints + 1);
	point_new.resize(nCircumPoints + 1);
	colors.resize(nCircumPoints + 1);
	// center point
	points[0] = vec2(0, 0);
	point_new[0] = vec2(0, 0);
	colors[0] = vec3(1, 1, 1);
	// circumference points
	for (int i = 0; i < nCircumPoints; i++) {
		float t = (float)i / (nCircumPoints - 1), a = 2 * 3.141592f * t;
		points[i + 1] = vec2(sin(a), cos(a));
		//colors[i+1] = HSVtoRGB(vec3(t, 1, 1));
	}

	// triangles
	triangles.resize(nCircumPoints);
	for (int i = 0; i < nCircumPoints; i++)
		triangles[i] = int3(0, i + 1, i == nCircumPoints - 1 ? 1 : i + 2);
	for (int i = 0; i < (int)triangles.size(); i++)
		int3 t = triangles[i];


	// *** JB: end new code
		// create GPU buffer, make it active, allocate memory and copy vertices
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	// allocate and fill vertex buffer
	// *** JB int vsize = sizeof(points), csize = sizeof(colors);
	int vsize = sizeof(vec2) * points.size(), csize = sizeof(vec3) * colors.size();
	int vsize2 = sizeof(vec2) * point_new.size(), csize1 = sizeof(vec3) * colors.size();// *** JB computed differently for dynamic arrays

	glBufferData(GL_ARRAY_BUFFER, vsize + csize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, points.data()); // *** JB: note difference between static and dynamic arrays
	glBufferSubData(GL_ARRAY_BUFFER, vsize, csize, colors.data());
}
// display
void MouseWheel(GLFWwindow* window, double ignore, double spin) {
	scale *= spin > 0 ? 1.1f : .9f;
}




void Display() {
	// clear screen to grey
	if (animate) {
		float dt = (float)(clock() - startTime) / CLOCKS_PER_SEC;		// elapsed time
		float freq = .5f, ang = 2 * 3.1415f * dt * freq;					// current angle
		scale = (1 + sin(ang)) / 2;
	}
	SetUniform(program, "scale", scale);
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	int vsize = sizeof(vec2)*points.size(); // *** JB vsize computed differently for dynamic array
	// *** JB: uncommented following
	VertexAttribPointer(program, "point", 2, 0, (void *) 0);
	//VertexAttribPointer(program, "color", 3, 0, (void *) vsize);
	
	glDrawElements(GL_TRIANGLES, 3*triangles.size(), GL_UNSIGNED_INT, triangles.data());
	glFlush();
}

// application

void Close() {
	// unbind vertex buffer and free GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vBuffer >= 0)
		glDeleteBuffers(1, &vBuffer);
}

//void ErrorGFLW(int id, const char *reason) {
//	printf("GFLW error %i: %s\n", id, reason);
//}

//void Keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//}


int main() {
//	glfwSetErrorCallback(ErrorGFLW); // *** JB: not really needed - simplify
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_SAMPLES, 4); // *** JB: smooth edges
	GLFWwindow *window = glfwCreateWindow(600, 600, "Emoji-Test-2", NULL, NULL); // *** JB new title
//	GLFWwindow *window = glfwCreateWindow(600, 600, "Colorful Letter", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
//	printf("GL version: %s\n", glGetString(GL_VERSION)); // *** JB: not really needed anymore
//	PrintGLErrors(); // *** JB: not really needed - simplify
	if (!(program = LinkProgramViaCode(&vertexShader, &pixelShader)))
		getchar();
	InitCircleBuffer(numberOfVertices);
	InitEyesBuffer(numberOfVertices1);
//	glfwSetKeyCallback(window, Keyboard); // *** JB: not needed
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		
		Display();
		
		//drawCircle(650 / 2, 650 / 2, 0, 120, 100);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Close();
	glfwDestroyWindow(window);
	glfwTerminate();
}

