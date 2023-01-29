// Emoji Expresser Team: display Emoji face on a background image
//                       the emoji changes in response to user action

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "Draw.h"
#include "GLXtras.h"
#include "Misc.h"
#include "Numbers.h"
#include "Sprite.h"

using namespace std;

// Application Data
int winWidth = 700, winHeight = 700;
bool showLines = false, showNumbers = false;

// Animatable Shape Class
class Shape {
public:
	int npoints = 0;
	vector<vec2> temp;
	GLuint vBuffer = 0;
	vector<int3> triangles;
	vec3 color;
	vec2 location;
	float scale = 1;
	GLuint program = 0;
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
	void Initialize(vec2* points, int npts, vec2 loc, float s, vec3 col) {
		npoints = npts;
		location = loc;
		scale = s;
		color = col;
		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, npoints * sizeof(vec2), NULL, GL_STATIC_DRAW);
		temp.resize(npoints);
		for (int i = 0; i < npoints; i++)
			temp[i] = points[i];
		UpdateVertices();
		Triangulate();
	}
	void UpdateVertices() {
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, npoints*sizeof(vec2), temp.data());
	}
	mat4 GetMatrix() { return Translate(location.x, location.y, 0) * Scale(scale, scale, 1); }
	void Triangulate() {
		int nids = npoints;
		vector<int> ids(nids);
		for (int i = 0; i < nids; i++) ids[i] = i;
		triangles.resize(0);
		while (ids.size()) {
			if (ids.size() == 3) {
				triangles.push_back(int3(ids[0], ids[1], ids[2]));
				return;
			}
			int i = MostAccute(ids);
			int i0 = ids[i ? i - 1 : nids - 1], i1 = ids[i], i2 = ids[i < nids - 1 ? i + 1 : 0];
			triangles.push_back(int3(i0, i1, i2));
			ids.erase(ids.begin() + i);
			nids--;
		}
	}
	void Display(bool lines = false, bool numbers = false, float lineWidth = 1) {
		if (!program)
			program = LinkProgramViaCode(&vertexShader, &pixelShader);
		glUseProgram(program);
		// Creating vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		VertexAttribPointer(program, "point", 2, 0, (void*)0); 
		SetUniform(program, "color", color);
		mat4 m = GetMatrix();
		SetUniform(program, "m", m);
		if (lines) {
			glLineWidth(lineWidth);
			for (int i = 0; i < (int) triangles.size(); i++)
				glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, &triangles[i]);
			glPointSize(9);
			glDrawElements(GL_POINTS, 3*triangles.size(), GL_UNSIGNED_INT, triangles.data());
		}
		else
			glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, triangles.data());
		if (showNumbers)
			for (int i = 0; i < npoints; i++) {
				vec2 s = ScreenPoint(vec3(temp[i], 0), m);
				Number((int) s.x, (int) s.y, i);
			}
	}
	int MostAccute(vector<int>& ids) {
		float maxDot = -1;
		int nids = ids.size(), maxI = 0;
		for (int i = 0; i < nids; i++) {
			int i0 = ids[i ? i - 1 : nids - 1], i1 = ids[i], i2 = ids[i < nids - 1 ? i + 1 : 0];
			vec2 p0 = temp[i0], p1 = temp[i1], p2 = temp[i2];
			vec2 v1 = normalize(p1 - p0), v2 = normalize(p1 - p2);
			float d = dot(v1, v2);
			if (d > maxDot && cross(v1, v2) < 0) { maxDot = d; maxI = i; }
		}
		return maxI;
	}
	void Interpolate(vec2* pts1, vec2* pts2, float a) {
		temp.resize(npoints);
		for (int i = 0; i < npoints; i++)
			temp[i] = pts1[i] + a * (pts2[i] - pts1[i]); // create a new set of points
		Triangulate();
		UpdateVertices();
	}
	~Shape() { if (vBuffer > 0) glDeleteBuffers(1, &vBuffer); }
};

Shape face, lEye, rEye, mouth, righteyebrow, lefteyebrow;

// Outlines
vec2 smile[] = { {-1.4, 1.18},  {-1.2, 0.9},   {-1.1, 0.7},  {-1.03, 0.6},  {-.95, 0.5},    {-.87, .4},    {-.8, .3},   {-.72, .2},
				 {-.6, .1},  {-.5, -0.0}, {-.4, -.1},  {-.2, -.23}, {-0.1, -0.3},   {0.0, -.33},   {0.2, -.32},   {.3, -.3},
				 {.5, -.2},    {.6, -0.1},   {.7, 0.0},   {.75, 0.1},   {.83, .2},    {.9, .35},    {0.95, 0.5},    {1.0, .65},
				 {1.1, 0.85},    {1.2, 1.05},  {1.3, 1.18}, {1.18, 1.1}, {1.05, .95},  {.9, .85},    {.75, .75},   {.55, .65},
				 {.33, .55},    {.15, .5},    {-.3, .6},    {-.5, .65},  {-.7, .75},   {-.9, .85},   {-1.1, 1.},  {-1.28, 1.1} };
vec2 frown[] = { {-1.1, .35},   {-1.2, .25},  {-1.28, .15}, {-1.3, .05}, {-1.3, -.05}, {-1.3, -.15}, {-1.25, -.3}, {-1.2, -.4},
				 {-1.15, -.45}, {-1.05, -.5}, {-.8, -.4},   {-.65, -.3}, {-.45, -.2},  {-.2, -.15},  {.05, -.2},   {.3, -.25},
				 {.55, -.4},    {.7, -.5},    {.85, -.4},   {.9, -.3},   {.95, -.25},  {1., -.15},   {1, 0},       {1., .1},
				 {.97, .2},     {.93, .3},    {.9, .4},     {.84, .48},  {.75, .5},    {.6, .53},    {.5, .45},    {.35, .4},
				 {.2, .3},      {.05, .25},   {-.2, .23},   {-.4, .35},  {-.55, .45},  {-.7, .5},    {-.9, .5},    {-1., .4} };
vec2 right_eyebrow[] = { {-1.0, -0.44},   {-0.9, -0.4},  {-0.8, -0.38}, {-0.7, -0.36}, {-0.6, -0.35}, {-0.5, -0.35}, {-0.4, -0.35}, {-0.3, -0.36},
				 {-0.2, -0.37}, {-0.1, -0.38}, {-0.0, -0.4},   {0.1, -0.42}, {0.2, -0.44},  {-1.05, -0.44},  {-1.10, -0.42},   {-1.15, -0.40},
				 {-1.10, -0.36},    {-1.05, -0.33},    {-0.95, -0.28},   {-0.85, -0.24},   {-0.75, -0.22},  {-0.65, -0.20},   {-0.60, -0.19},       {-0.55 , -0.18},
				 {-0.50, -0.18},     {-0.45, -0.18},    {-0.40, -0.18},     {-0.35, -0.19},  {-0.30, -0.20},    {-0.25, -0.21},    {-0.20, -0.22},    {-0.15, -0.24},
				 {-0.10,-0.26},      {-0.05, -0.28},   {0.0,-0.30},   {0.05, -0.32},  {0.10, -0.34},  {0.15, -0.36},    {0.20, -0.38},    {0.25, -0.40} };

vec2 right_eyebrow_sad[] = { {-1.0, -0.84},   {-0.9, -0.8},  {-0.8, -0.78}, {-0.7, -0.76}, {-0.6, -0.75}, {-0.5, -0.75}, {-0.4, -0.75}, {-0.3, -0.76},
				 {-0.2, -0.77}, {-0.1, -0.78}, {-0.0, -0.8},   {0.1, -0.82}, {0.2, -0.84},  {-1.05, -0.84},  {-1.10, -0.82},   {-1.15, -0.80},
				 {-1.10, -0.76},    {-1.05, -0.73},    {-0.95, -0.68},   {-0.85, -0.64},   {-0.75, -0.62},  {-0.65, -0.60},   {-0.60, -0.59},       {-0.55 , -0.58},
				 {-0.50, -0.58},     {-0.45, -0.58},    {-0.40, -0.58},     {-0.35, -0.59},  {-0.30, -0.60},    {-0.25, -0.61},    {-0.20, -0.62},    {-0.15, -0.64},
				 {-0.10,-0.66},      {-0.05, -0.68},   {0.0,-0.70},   {0.05, -0.72},  {0.10, -0.74},  {0.15, -0.76},    {0.20, -0.78},    {0.25, -0.80} };

// Sprites
Sprite background, slapButton;
Sprite *buttons[] = { &slapButton };
int nbuttons = sizeof(buttons)/sizeof(Sprite *);

// Actions
enum Action { Slap = 0, Tickle, Poke, None };
const char *actionNames[] = { "Slap", "Tickle", "Poke", "None" };
Action action = None;

// Filenames
string dir = "C:/Users/mosta/Graphics/Apps/";
string backgroundTex = "emoji.tga";
string slapButtonTex = "slapEmoji.jpg";

const int npts = 40;
vec2 circle[npts];

void InitShapes() {
	for (int i = 0; i < npts; i++) {
		float t = (float)i / (npts - 1), a = 2 * 3.141592f * t;
		circle[i] = vec2(cos(a), sin(a));
	}
	face.Initialize(circle, npts, vec2(0, 0), 1, vec3(1, 1, 0));
	lEye.Initialize(circle, npts, vec2(-.4f, .25f), .1f, vec3(.7f, 0, 0));
	rEye.Initialize(circle, npts, vec2(.4f, .25f), .1f, vec3(.7f, 0, 0));
	mouth.Initialize(smile, npts, vec2(0.f, -.5f), .5f, vec3(.7f, 0, 0));
	righteyebrow.Initialize(right_eyebrow, npts, vec2(-.25f, .7f), .3f, vec3(.7f, 0, 0));
	lefteyebrow.Initialize(right_eyebrow, npts, vec2(.45f, .7f), .3f, vec3(.7f, 0, 0));
}

// Animation

void NewAction(Action a) {
	action = a;
	printf("NewAction = %s!\n", actionNames[a]);
}

bool animating = false, smiling = true;
float animationDuration = .5f;
time_t animationStart;

void Animate() {
	if (!animating) animationStart = clock();
	if (animating) {
		float dt = (float)(clock() - animationStart) / CLOCKS_PER_SEC;
		if (dt > animationDuration) {
			animating = false;
			smiling = !smiling;
		}
		else {
			float a = dt / animationDuration, t = smiling ? a : 1 - a;
			mouth.Interpolate(smile, frown, t);
			righteyebrow.Interpolate(right_eyebrow, right_eyebrow_sad, t);
			lefteyebrow.Interpolate(right_eyebrow, right_eyebrow_sad, t);
		}
	}
}

// Display

void Display() {
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	background.Display();
	// display emoji
	glDisable(GL_DEPTH_TEST);
	face.Display(showLines, showNumbers, 1);
	lEye.Display(showLines, showNumbers, 1);
	rEye.Display(showLines, showNumbers, 1);
	mouth.Display(showLines, showNumbers, 3);
	righteyebrow.Display(showLines, showNumbers, 1);
	lefteyebrow.Display(showLines, showNumbers, 1);
	glEnable(GL_DEPTH_TEST);
	slapButton.Display();
	glFlush();
}

// Interaction

void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		switch(key) {
		case 'L': showLines = !showLines; break;
		case 'N': showNumbers = !showNumbers; break;
		default: animating = true;
	}
}

void MouseButton(GLFWwindow* w, int butn, int action, int mods) {
	double x, y;
	glfwGetCursorPos(w, &x, &y);
	y = winHeight-y; // invert y for upward-increasing screen space
	if (action == GLFW_PRESS) {
		for (int i = 0; i < nbuttons; i++)
			if (buttons[i]->Hit((int) x, (int) y))
				NewAction((Action) i);
	}
}

// Application

void Resize(GLFWwindow* w, int width, int height) {
	winWidth = width;
	winHeight = height;
	glViewport(0, 0, width, height);
}

const char *usage = R"(Options:
	L: toggle lines
	N: toggle numbers
	any other letter: animate
)";

int main() {
	// initialize window
	if (!glfwInit()) return 1;
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* w = glfwCreateWindow(winWidth, winHeight, "EmojiExpresser", NULL, NULL);
	if (!w) { glfwTerminate(); return 1; }
	glfwSetWindowPos(w, 200, 200);
	// callbacks
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	// initialize OpenGL, shader program, shapes
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	background.Initialize(dir+backgroundTex, "", 0, .7f);
	slapButton.Initialize(dir+slapButtonTex, 1, .5f);
	slapButton.SetScale(vec2(.075f));
	slapButton.SetPosition(vec2(.8f, -.8f));
	InitShapes();
	printf(usage);
	// event loop
	glfwSetMouseButtonCallback(w, MouseButton);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		Animate();
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	background.Release();
	slapButton.Release();
	glfwDestroyWindow(w);
	glfwTerminate();
}
