// PointMeasure: measure points on image

#include <glad.h>
#include <glfw3.h>
#include <string.h>
#include <cstdio>
#include <vector>
#include "Draw.h"
#include "Sprite.h"

using namespace std;

namespace {

// Image
Sprite texture;
string dir = "C:/Users/mosta/Graphics/Apps/";
string image = "wow-emoji.png";

// App Window, Interaction
int winWidth = 700, winHeight = 800;
int selected = -1;

// Outline

vector<vec2> outline;

string GetFilename(const char *prompt) {
	char buf[1000] = { 0 };
	printf(prompt);
	fgets(buf, 100, stdin);
	if (strlen(buf))
		buf[strlen(buf)-1] = 0; // remove <CR>
	return dir+string(buf);
}

void PrintOutline() {
	string filename = GetFilename("type file to write: ");
	const char *s = filename.c_str();
	FILE *file = fopen(s, "w");
	if (file) {
		int npoints = outline.size();
		for (int i = 0; i < npoints; i++)
			fprintf(file, " {%3.2f, %3.2f}%s", outline[i].x, outline[i].y, i==npoints-1? "\n" : (i+1)%8==0? ",\n" : ", ");
		fclose(file);
		printf("printed = %s\n", s);
	}
}

bool ReadWord(char* &ptr, char *word, int charLimit) {
	int skip = strcspn(ptr, "-.0123456789");		// skip to a number
	ptr += skip;
	int nChars = strspn(ptr, "-.0123456789");		// get # number characters
	if (!nChars)
		return false;								// no non-space characters
	int nRead = charLimit-1 < nChars? charLimit-1 : nChars;
	strncpy(word, ptr, nRead);
	word[nRead] = 0;								// strncpy does not null terminate
	ptr += nChars;
	return true;
}

void InputOutline() {
	string filename = GetFilename("type file to read: ");
	const char *s = filename.c_str();
	FILE *in = fopen(s, "r");
	if (!in)
		printf("can't open %s\n", s);
	else {
		const int LineLim = 1000, WordLim = 1000;
		char line[LineLim], word1[WordLim], word2[WordLim];
		outline.resize(0);
		for (int lineNum = 0;; lineNum++) {
			line[0] = 0;
			fgets(line, LineLim, in);
			if (feof(in))
				break;
			char *ptr = line;
			while (ReadWord(ptr, word1, WordLim) && ReadWord(ptr, word2, WordLim))
				outline.push_back(vec2((float) atof(word1), (float) atof(word2)));
		}
		fclose(in);
		printf("read %i points from %s\n", outline.size(), s);
	}
}

} // end namespace

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
		switch (key) {
			case 'P': PrintOutline(); break;
			case 'I': InputOutline(); break;
			case 'R': outline.resize(0); break;
		}
}

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, winWidth = width, winHeight = height);
}

const char *usage = R"(Options:
	P: print points
	I: input points
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
