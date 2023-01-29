/* Emoji Expresser Team: This code displays Emoji face on a background image and the emoji changes its smile on pressing any key, 
						 emoji faces fades with lines on pressing "F" */
#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "GLXtras.h"
#include "Misc.h"
#include "Sprite.h"

using std::vector;

//OpenGL Shader Program

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

//Shape class holds the animatable shapes

class Shape {
public:
	int npoints = 0;
	GLuint vBuffer = 0;
	vector<int3> triangles;
	vec3 color;
	vec2 location;
	float scale = 1;
	
	// Initializing the buffer for the points of the shapes
	void InitializeEmoji(vec2* points, int npts, vec2 loc, float s, vec3 col) {
		npoints = npts;
		location = loc;
		scale = s;
		color = col;
		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, npoints * sizeof(vec2), NULL, GL_STATIC_DRAW);
		UpdateVertices(points);
	}
	void UpdateVertices(vec2* points) {
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, npoints * sizeof(vec2), points);
		Triangulate(points);
	}
	// Triangulation of the most accute points
	void Triangulate(vec2* points) {
		int nids = npoints;
		vector<int> ids(nids);
		for (int i = 0; i < nids; i++) ids[i] = i;
		triangles.resize(0);
		while (ids.size()) {
			if (ids.size() == 3) {
				triangles.push_back(int3(ids[0], ids[1], ids[2]));
				return;
			}
			int i = MostAccute(ids, points);
			int i0 = ids[i ? i - 1 : nids - 1], i1 = ids[i], i2 = ids[i < nids - 1 ? i + 1 : 0];
			triangles.push_back(int3(i0, i1, i2));
			ids.erase(ids.begin() + i);
			nids--;
		}
	}
	// Utility function to display the shapes
	void Display(bool lines = false) {
		glUseProgram(program);
		// Creating vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		VertexAttribPointer(program, "point", 2, 0, (void*)0); 
		SetUniform(program, "color", color);
		SetUniform(program, "m", Translate(location.x, location.y, 0) * Scale(scale, scale, 1));
		if (lines)
			for (int i = 0; i < (int)triangles.size(); i++)
				glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, &triangles[i]);
		else
			glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, triangles.data());
	}
	int MostAccute(vector<int>& ids, vec2* points) {
		float maxDot = -1;
		int nids = ids.size(), maxI = 0;
		for (int i = 0; i < nids; i++) {
			int i0 = ids[i ? i - 1 : nids - 1], i1 = ids[i], i2 = ids[i < nids - 1 ? i + 1 : 0];
			vec2 p0 = points[i0], p1 = points[i1], p2 = points[i2];
			vec2 v1 = normalize(p1 - p0), v2 = normalize(p1 - p2);
			float d = dot(v1, v2);
			if (d > maxDot && cross(v1, v2) < 0) { maxDot = d; maxI = i; }
		}
		return maxI;
	}
	// Function for the interpolation of shapes
	void Interpolate(vec2* pts1, vec2* pts2, float a) {
		vector<vec2> temp(npoints);
		for (int i = 0; i < npoints; i++)
			temp[i] = pts1[i] + a * (pts2[i] - pts1[i]); // create a new set of points
		Triangulate(temp.data());
		UpdateVertices(temp.data());
	}
	~Shape() { if (vBuffer > 0) glDeleteBuffers(1, &vBuffer); }
};

Shape face, lEye, rEye, mouth, righteyebrow, lefteyebrow;

// Shape Initialization points
vec2 smile[] = {
 {-0.63, 0.47},
 {-0.62, 0.50},
 {-0.61, 0.51},
 {-0.60, 0.53},
 {-0.58, 0.55},
 {-0.57, 0.56},
 {-0.56, 0.57},
 {-0.55, 0.58},
 {-0.53, 0.59},
 {-0.52, 0.60},
 {-0.51, 0.61},
 {-0.50, 0.61},
 {-0.49, 0.61},
 {-0.48, 0.61},
 {-0.46, 0.62},
 {-0.45, 0.62},
 {-0.44, 0.61},
 {-0.43, 0.61},
 {-0.41, 0.61},
 {-0.40, 0.60},
 {-0.38, 0.60},
 {-0.37, 0.59},
 {-0.37, 0.56},
 {-0.38, 0.55},
 {-0.39, 0.55},
 {-0.41, 0.55},
 {-0.42, 0.56},
 {-0.44, 0.56},
 {-0.45, 0.56},
 {-0.47, 0.56},
 {-0.48, 0.56},
 {-0.49, 0.55},
 {-0.51, 0.54},
 {-0.52, 0.54},
 {-0.53, 0.52},
 {-0.55, 0.51},
 {-0.56, 0.50},
 {-0.57, 0.48},
 {-0.59, 0.46},
 {-0.60, 0.44},
 {-0.61, 0.44},
 {-0.62, 0.45}
};


//vec2 smile[] = { {-1.4, 1.18},  {-1.2, 0.9},   {-1.1, 0.7},  {-1.03, 0.6},  {-.95, 0.5},    {-.87, .4},    {-.8, .3},   {-.72, .2},
//				 {-.6, .1},  {-.5, -0.0}, {-.4, -.1},  {-.2, -.23}, {-0.1, -0.3},   {0.0, -.33},   {0.2, -.32},   {.3, -.3},
//				 {.5, -.2},    {.6, -0.1},   {.7, 0.0},   {.75, 0.1},   {.83, .2},    {.9, .35},    {0.95, 0.5},    {1.0, .65},
//				 {1.1, 0.85},    {1.2, 1.05},  {1.3, 1.18}, {1.18, 1.1}, {1.05, .95},  {.9, .85},    {.75, .75},   {.55, .65},
//				 {.33, .55},    {.15, .5},    {-.3, .6},    {-.5, .65},  {-.7, .75},   {-.9, .85},   {-1.1, 1.},  {-1.28, 1.1} };
vec2 suprise[] = { {-1.1, .35},   {-1.2, .25},  {-1.28, .15}, {-1.3, .05}, {-1.3, -.05}, {-1.3, -.15}, {-1.25, -.3}, {-1.2, -.4},
				 {-1.15, -.45}, {-1.05, -.5}, {-1.05, -.4},   {-1.05, -.3}, {-1.05, -.2},  {-1.05, -.15},  {-1.05, -.2},   {-1.05, -.25},
				 {-1.05, -.4},    {-1.05, -.5},    {.85, -.4},   {.9, -.3},   {.95, -.25},  {1., -.15},   {1, 0},       {1., .1},
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

Sprite background, * selected = NULL;
//Update line 147 with the path for the background image
string dir = "C:/Users/saran/Graphics/Apps/";
string backgroundTex = dir + "emoji.tga";

const int npts = 40;
vec2 circle[npts];

// Initialize emoji shapes
void InitShapes() {
	for (int i = 0; i < npts; i++) {
		float t = (float)i / (npts - 1), a = 2 * 3.141592f * t;
		circle[i] = vec2(cos(a), sin(a));
	}
	face.InitializeEmoji(circle, npts, vec2(0, 0), 1, vec3(1, 1, 0));
	lEye.InitializeEmoji(circle, npts, vec2(-.4f, .25f), .1f, vec3(.7f, 0, 0));
	rEye.InitializeEmoji(circle, npts, vec2(.4f, .25f), .1f, vec3(.7f, 0, 0));
	mouth.InitializeEmoji(smile, npts, vec2(0.f, -.5f), .5f, vec3(.7f, 0, 0));
	righteyebrow.InitializeEmoji(right_eyebrow, npts, vec2(-.25f, .7f), .3f, vec3(.7f, 0, 0));
	lefteyebrow.InitializeEmoji(right_eyebrow, npts, vec2(.45f, .7f), .3f, vec3(.7f, 0, 0));
}

// Animation of the emoji

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
			mouth.Interpolate(smile, suprise, t);
			righteyebrow.Interpolate(right_eyebrow, right_eyebrow_sad, t);
			lefteyebrow.Interpolate(right_eyebrow, right_eyebrow_sad, t);
		}
	}
}

// Utility function to display emoji

bool showLines = false;

void Display() {
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(1);
	background.Display();
	face.Display(showLines);
	lEye.Display(showLines);
	rEye.Display(showLines);
	glLineWidth(2);
	mouth.Display(showLines);
	righteyebrow.Display(showLines);
	lefteyebrow.Display(showLines);
	glFlush();
}

// Function for Keyboard press action

void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == 'F') showLines = !showLines;
		else animating = true;
	}
}

void MouseButton(GLFWwindow* w, int butn, int action, int mods) {
	double x, y;
	glfwGetCursorPos(w, &x, &y);
	y = 700 - y; // invert y for upward-increasing screen space
	if (action == GLFW_PRESS) {
		int ix = (int)x, iy = (int)y;
		selected = NULL;
		if (selected)
			selected->MouseDown(vec2((float)x, (float)y));
	}
}

void MouseMove(GLFWwindow* w, double x, double y) {
	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && selected)
		selected->MouseDrag(vec2((float)x, (float)(700 - y)));
}

void MouseWheel(GLFWwindow* w, double ignore, double spin) {
	if (selected)
		selected->MouseWheel(spin);
}

// To display as per the window maximize and minimize

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}




int main() {
	// initialize window
	if (!glfwInit()) return 1;
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* w = glfwCreateWindow(700, 700, "EmojiExpresser", NULL, NULL);
	if (!w) { glfwTerminate(); return 1; }
	glfwSetWindowPos(w, 200, 200);
	// callbacks
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	// initialize OpenGL, shader program, shapes
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	background.Initialize(backgroundTex, "", 0, .7f);
	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	InitShapes();
	printf("Want to make Emoji express its feeling???\n");
	printf("Type F to Fade with lines, any other key to animate the smiling emoji\n");
	// event loop
	glfwSetMouseButtonCallback(w, MouseButton);
	glfwSetCursorPosCallback(w, MouseMove);
	glfwSetScrollCallback(w, MouseWheel);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		Animate();
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	background.Release();
	glfwDestroyWindow(w);
	glfwTerminate();
}
