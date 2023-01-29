// Emoji-TriangulateTest.cpp: draw a polygon as multiple triangles

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include <time.h>
#include <vector>

// OpenGL Identifiers
GLuint vBuffer = 0, program = 0;

// Application Parameters
int winWidth = 600, winHeight = 600;
bool showLines = false;

// Shaders

const char *vertexShader = R"(
	#version 130
	uniform mat4 m;
	in vec2 point;
	void main() {
		gl_Position = m*vec4(point, 0, 1);
	}
)";

const char *pixelShader = R"(
	#version 130
	uniform vec3 color;
	out vec4 pColor;
	void main() {
		pColor = vec4(color, 1);
	}
)";

// Smile/Frown of 40 Points
vec2 smile[] = { {-.0, 1.2},  {-1.3, 1.},   {-1.15, .9},  {-1., .75},  {-.9, .6},    {-.8, .4},    {-.75, .2},   {-.7, .05},
				 {-.65, -.15},  {-.55, -.35}, {-.4, -.45},  {-.3, -.43}, {-.2, -.4},   {-.1, -.3},   {0., -.32},   {.1, -.38},
				 {.25, -.4},    {.5, -.38},   {.6, -.24},   {.75, 0.},   {.83, .2},    {.9, .35},    {.95, .5},    {1., .65},
				 {1.1, .85},    {1.2, 1.05},  {1.35, 1.23}, {1.18, 1.1}, {1.05, .95},  {.9, .85},    {.75, .75},   {.55, .65},
				 {.33, .55},    {.15, .5},    {-.3, .6},    {-.5, .65},  {-.7, .75},   {-.9, .85},   {-1.15, 1.},  {-1.28, 1.1} };
vec2 frown[] = { {-1.1, .35},   {-1.2, .25},  {-1.28, .15}, {-1.3, .05}, {-1.3, -.05}, {-1.3, -.15}, {-1.25, -.3}, {-1.2, -.4},
				 {-1.15, -.45}, {-1.05, -.5}, {-.8, -.4},   {-.65, -.3}, {-.45, -.2},  {-.2, -.15},  {.05, -.2},   {.3, -.25},
				 {.55, -.4},    {.7, -.5},    {.85, -.4},   {.9, -.3},   {.95, -.25},  {1., -.15},   {1, 0},       {1., .1},
				 {.97, .2},     {.93, .3},    {.9, .4},     {.84, .48},  {.75, .5},    {.6, .53},    {.5, .45},    {.35, .4},
				 {.2, .3},      {.05, .25},   {-.2, .23},   {-.4, .35},  {-.55, .45},  {-.7, .5},    {-.9, .5},    {-1., .4} };
const int npts = sizeof(smile)/sizeof(vec2);
vec2 frile[npts];

std::vector<int3> triangles;

void AllocateVertexBuffer() {
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smile), NULL, GL_STATIC_DRAW);
}

void UpdateVertexBuffer(vec2 *pts) {
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, npts*sizeof(vec2), pts);
}

// Triangulation

int MostAccute(std::vector<int> &ids, vec2 *pts) {
	float maxDot = -1;
	int nids = ids.size(), maxI = 0;
	for (int i = 0; i < nids; i++) {
		int i0 = ids[i? i-1 : nids-1], i1 = ids[i], i2 = ids[i < nids-1? i+1 : 0];
		vec2 p0 = pts[i0], p1 = pts[i1], p2 = pts[i2];
		vec2 v1 = normalize(p1-p0), v2 = normalize(p1-p2);
		float d = dot(v1, v2);
		if (d > maxDot) { maxDot = d; maxI = i; }
	}
	return maxI;
}

void SetTriangles(vec2 *pts, int nids) {
	std::vector<int> ids(nids);
	for (int i = 0; i < nids; i++) ids[i] = i;
	triangles.resize(0);
	while(ids.size()) {
		if (ids.size() == 3) {
			triangles.push_back(int3(ids[0], ids[1], ids[2]));
			return;
		}
		int i = MostAccute(ids, pts);
		int i0 = ids[i? i-1 : nids-1], i1 = ids[i], i2 = ids[i < nids-1? i+1 : 0];
		triangles.push_back(int3(i0, i1, i2));
		ids.erase(ids.begin()+i);
		nids--;
	}
}

// Animation

bool animating = false, smiling = true;
time_t animationStart;

void BeginAnimation() {
	if (!animating) {
		animating = true;
		animationStart = clock();
	}
}

vec2 Interpolate(vec2 p1, vec2 p2, float t) { return p1+t*(p2-p1); }

void Animate() {
	if (animating) {
		float dt = (float)(clock()-animationStart)/CLOCKS_PER_SEC, period = .5f;
		if (dt > period) {
			animating = false;
			smiling = !smiling;
			return;
		}
		float a = dt/period;
		for (int i = 0; i < npts; i++)
			frile[i] = smiling?
				Interpolate(smile[i], frown[i], a) :
				Interpolate(frown[i], smile[i], a);
		UpdateVertexBuffer(frile);
		SetTriangles(frile, npts);
	}
}

// Display

void Display() {
	// clear screen to grey
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	VertexAttribPointer(program, "point", 2, 0, (void *) 0);
	SetUniform(program, "color", vec3(0,1,0));
	SetUniform(program, "m", Scale(.7f,.7f,.7f));
	if (showLines)
		for (int i = 0; i < (int) triangles.size(); i++)
			glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, &triangles[i]);
	else
		glDrawElements(GL_TRIANGLES, 3*triangles.size(), GL_UNSIGNED_INT, triangles.data());
	glFlush();
}

// Application

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

void Keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (!animating)
		BeginAnimation();
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow *w = glfwCreateWindow(winWidth, winHeight, "Emoji-Test-2", NULL, NULL);
	if (!w) {
		glfwTerminate();
		return 1;
	}
	glfwSetWindowPos(w, 200, 200);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	AllocateVertexBuffer();
	UpdateVertexBuffer(smile);
	SetTriangles(smile, npts);
	glfwSwapInterval(1);
	printf("Type a key!\n");
	while (!glfwWindowShouldClose(w)) {
		Animate();
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	// unbind vertex buffer, free GPU memory, kill window
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vBuffer >= 0)
		glDeleteBuffers(1, &vBuffer);
	glfwDestroyWindow(w);
	glfwTerminate();
}

