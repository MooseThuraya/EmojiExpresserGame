// EmojiEyesMouth.cpp

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "GLXtras.h"
#include "Misc.h"
#include "Sprite.h"

using std::vector;


// OpenGL Shader Program

GLuint program = 0;

const char *vertexShader = R"(
	#version 130
	in vec2 point;
	uniform mat4 m = mat4(1);
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

// Animatable Shapes

class Shape {
public:
	int npoints = 0;
	GLuint vBuffer = 0;
	vector<int3> triangles;
	vec3 color;
	vec2 location;
	float scale = 1;
	void InitializeEmoji(vec2 *points, int npts, vec2 loc, float s, vec3 col) {
		npoints = npts;
		location = loc;
		scale = s;
		color = col;
		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, npoints*sizeof(vec2), NULL, GL_STATIC_DRAW);
		UpdateVertices(points);
	}
	void UpdateVertices(vec2 *points) {
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, npoints*sizeof(vec2), points);
		Triangulate(points);
	}
	void Triangulate(vec2 *points) {
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
			int i0 = ids[i? i-1 : nids-1], i1 = ids[i], i2 = ids[i < nids-1? i+1 : 0];
			triangles.push_back(int3(i0, i1, i2));
			ids.erase(ids.begin()+i);
			nids--;
		}
	}
	void Display(bool lines = false) {
		glUseProgram(program);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer); // create vertex buffer
		VertexAttribPointer(program, "point", 2, 0, (void *) 0); // connect to shader
		SetUniform(program, "color", color);
		SetUniform(program, "m", Translate(location.x, location.y, 0)*Scale(scale, scale, 1)); // translate and multiply by scale
		if (lines)
			for (int i = 0; i < (int) triangles.size(); i++)
				glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, &triangles[i]);
		else
			glDrawElements(GL_TRIANGLES, 3*triangles.size(), GL_UNSIGNED_INT, triangles.data());
	}
	int MostAccute(vector<int> &ids, vec2 *points) {
		float maxDot = -1;
		int nids = ids.size(), maxI = 0;
		for (int i = 0; i < nids; i++) {
			int i0 = ids[i? i-1 : nids-1], i1 = ids[i], i2 = ids[i < nids-1? i+1 : 0];
			vec2 p0 = points[i0], p1 = points[i1], p2 = points[i2];
			vec2 v1 = normalize(p1-p0), v2 = normalize(p1-p2);
			float d = dot(v1, v2);
			if (d > maxDot && cross(v1, v2) < 0) { maxDot = d; maxI = i; }
		}
		return maxI;
	}
	void Interpolate(vec2 *pts1, vec2 *pts2, float a) {
		vector<vec2> temp(npoints);
		for (int i = 0; i < npoints; i++)
			temp[i] = pts1[i]+a*(pts2[i]-pts1[i]); // create a new set of points
		Triangulate(temp.data());	
		UpdateVertices(temp.data());
	}
	~Shape() { if (vBuffer > 0) glDeleteBuffers(1, &vBuffer); }
};

Shape face, lEye, rEye, mouth;

// Shape Initialization

vec2 smile[] = { {-1.4, 1.18},  {-1.25, 0.95},   {-1.15, 0.75},  {-1.03, 0.6},  {-.95, 0.5},    {-.85, .4},    {-.75, .3},   {-.65, .2},
				 {-.55, .1},  {-.3, -.15}, {-.2, -.23},  {-.1, -.3}, {-0.0, -0.33},   {0.2, -.32},   {0.3, -.3},   {.5, -.2},
				 {.6, -.1},    {.7, 0.0},   {.75, 0.1},   {.83, 0.2},   {.9, .35},    {.95, .5},    {1.0, 0.65},    {1.1, .85},
				 {1.2, 1.05},    {1.2, 1.05},  {1.3, 1.18}, {1.18, 1.1}, {1.05, .95},  {.9, .85},    {.75, .75},   {.55, .65},
				 {.33, .55},    {.15, .5},    {-.3, .6},    {-.5, .65},  {-.7, .75},   {-.9, .85},   {-1.1, 1.},  {-1.28, 1.1} };
vec2 frown[] = { {-1.1, .35},   {-1.2, .25},  {-1.28, .15}, {-1.3, .05}, {-1.3, -.05}, {-1.3, -.15}, {-1.25, -.3}, {-1.2, -.4},
				 {-1.15, -.45}, {-1.05, -.5}, {-1.05, -.4},   {-1.05, -.3}, {-1.05, -.2},  {-1.05, -.15},  {-1.05, -.2},   {-1.05, -.25},
				 {-1.05, -.4},    {-1.05, -.5},    {.85, -.4},   {.9, -.3},   {.95, -.25},  {1., -.15},   {1, 0},       {1., .1},
				 {.97, .2},     {.93, .3},    {.9, .4},     {.84, .48},  {.75, .5},    {.6, .53},    {.5, .45},    {.35, .4},
				 {.2, .3},      {.05, .25},   {-.2, .23},   {-.4, .35},  {-.55, .45},  {-.7, .5},    {-.9, .5},    {-1., .4} };
//const int npts = sizeof(smile)/sizeof(vec2);



Sprite background, sprite1, sprite2, * selected = NULL;

string dir = "C:/Users/saran/Graphics/Apps/";
string sprite1Tex = dir + "Lily.tga", sprite1Mat = dir + "1.tga";
string sprite2Tex = dir + "YosemiteSam.tga", sprite2Mat = dir + "2.tga";
string combined32 = dir + "Combined32.png"; // png, tga ok; bmp, jpg do not support 32
string backgroundTex = dir + "Earth.tga";

const int npts = 40;
vec2 circle[npts];
//vec2 circle[40];

void InitShapes() {
	for (int i = 0; i < npts; i++) {
		float t = (float) i/(npts-1), a = 2*3.141592f*t;
		circle[i] = vec2(cos(a), sin(a));
	}
	face.InitializeEmoji(circle, npts, vec2(0, 0), 1, vec3(1, 1, 0));
	lEye.InitializeEmoji(circle, npts, vec2(-.4f, .25f), .1f, vec3(.7f, 0, 0));
	rEye.InitializeEmoji(circle, npts, vec2(.4f, .25f), .1f, vec3(.7f, 0, 0));
	mouth.InitializeEmoji(smile, npts, vec2(0.f, -.5f), .5f, vec3(.7f, 0, 0));
}

// Animation

bool animating = false, smiling = true;
float animationDuration = .5f;
time_t animationStart;

void Animate() {
	if (!animating) animationStart = clock();
	if (animating) {
		float dt = (float)(clock()-animationStart)/CLOCKS_PER_SEC;
		if (dt > animationDuration) {
			animating = false;
			smiling = !smiling;
		}
		else {
			float a = dt/animationDuration, t = smiling? a : 1-a;
			mouth.Interpolate(smile, frown, t);
		}
	}
}

// Display

bool showLines = false;

void Display() {
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1);
	background.Display();
	sprite1.Display();
	sprite2.Display(); 
	glDisable(GL_DEPTH_TEST);
	face.Display(showLines);
	lEye.Display(showLines);
	rEye.Display(showLines);
	glLineWidth(2);
	mouth.Display(showLines);
	glFlush();
}

// Application

void Keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == 'L') showLines = !showLines;
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
		if (sprite1.Hit(ix, iy)) selected = &sprite1;
		if (sprite2.Hit(ix, iy)) selected = &sprite2;
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


void Resize(GLFWwindow* w, int width, int height) {
	//glViewport(0, 0, width, height);
}




int main() {
	// initialize window
	if (!glfwInit()) return 1;
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* w = glfwCreateWindow(700, 700, "Emoji", NULL, NULL);
	if (!w) { glfwTerminate(); return 1; }
	glfwSetWindowPos(w, 150, 150);
	// callbacks
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	// initialize OpenGL, shader program, shapes
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	background.Initialize(backgroundTex, "", 0, .7f);
	sprite1.Initialize(combined32, 1, .2f);
	//	sprite1.Initialize(sprite1Tex, sprite1Mat, 1, .2f);
	sprite2.Initialize(sprite2Tex, sprite2Mat, 2, .1f);
	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	InitShapes();
	printf("type L to toggle lines, any other key to animate\n");
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
	sprite1.Release();
	sprite2.Release();
	glfwDestroyWindow(w);
	glfwTerminate();
}
