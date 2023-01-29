// 4-ColorfulLetter.cpp: draw multiple triangles to form a colorful letter

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include <vector>
#include <math.h>
#include <GL/glu.h>


const int numberOfVertices = 500;

// shader program

GLuint vBuffer = 0; // GPU vertex buffer ID, valid if > 0
GLuint program = 0; // GLSL program ID, valid if > 0

const char *vertexShader = R"(
	#version 130
	in vec2 point;
	in vec3 color;
	out vec4 vColor;
	void main() {
		gl_Position = vec4(point, 0, 1);
		vColor = vec4(color, 1);
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

// the letter T: 8 vertices, 8 colors, triangulation
float points[][2] = {{-.25f, -.75f}, {-.25f, .3f}, {-.75f, .3f}, {-.75f,  .75f},
					 { .75f,  .75f}, { .75f, .3f}, { .25f, .3f}, { .25f, -.75f}};
float colors[][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
					 {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
int triangles[][3] = {{0, 7, 1}, {7, 6, 1}, {1, 3, 2}, {1, 4, 3}, {1, 6, 4}, {6, 5, 4}}; // good
//int triangles[][3] = {{0, 7, 1}, {7, 6, 1}, {2, 4, 3}, {2, 5, 4}}; // bad

/* the letter B: 10 vertices, 10 colors, 9 triangles
s
float points[][2] = {{-.15f, .125f}, {-.5f, -.75f}, {-.5f,   .75f}, {.17f, .75f}, {.38f, .575f},
					 {.38f,   .35f}, {.23f, .125f}, {.5f,  -.125f}, {.5f,  -.5f}, {.25f, -.75f}};

float colors[][3] = {{1, 1, 1}, {1, 0, 0}, {.5f, 0, 0}, {1, 1, 0}, {.5f, 1, 0},
					 {0, 1, 0}, {0, 1, 1}, {0,   0, 1}, {1, 0, 1}, {.5f, 0, .5f}};

int triangles[][3] = {{0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5},
					  {0, 5, 6}, {0, 6, 7}, {0, 7, 8}, {0, 8, 9}, {0, 9, 1}}; */

void InitVertexBuffer() {
	// create GPU buffer, make it active, allocate memory and copy vertices
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	// allocate and fill vertex buffer
	int vsize = sizeof(points), csize = sizeof(colors);
	glBufferData(GL_ARRAY_BUFFER, vsize+csize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, points);
	glBufferSubData(GL_ARRAY_BUFFER, vsize, csize, colors);
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	//int numberOfVertices = numberOfSides + 2;



	GLfloat twicePi = 2.0f * 3.14158972;

	GLfloat circleVerticesX[numberOfVertices];
	GLfloat circleVerticesY[numberOfVertices];
	GLfloat circleVerticesZ[numberOfVertices];



	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;



	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}



	GLfloat allCircleVertices[(numberOfVertices) * 3];



	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}



	glEnableVertexAttribArray(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	glDisableVertexAttribArray(GL_VERTEX_ARRAY);
}
// display

void Display() {
	// clear screen to grey
	glClearColor(.5f, .5f, .5f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	drawCircle(650 / 2, 650 / 2, 0, 120, 100);
	// establish vertex fetch for point and for color, then draw triangles
	//int vsize = sizeof(points), ntris = sizeof(triangles)/(3*sizeof(int));
	//VertexAttribPointer(program, "point", 2, 0, (void *) 0);
	//VertexAttribPointer(program, "color", 3, 0, (void *) vsize);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	//glDisableClientState(GL_VERTEX_ARRAY);
	
	
	//glDrawElements(GL_TRIANGLES, 3*ntris, GL_UNSIGNED_INT, &triangles[0]);
	glFlush();
}

// application

void Close() {
	// unbind vertex buffer and free GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (vBuffer >= 0)
		glDeleteBuffers(1, &vBuffer);
}

void ErrorGFLW(int id, const char *reason) {
	printf("GFLW error %i: %s\n", id, reason);
}

void Keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}



int main() {
	glfwSetErrorCallback(ErrorGFLW);
	if (!glfwInit())
		return 1;
	GLFWwindow *window = glfwCreateWindow(600, 600, "Colorful Letter", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	printf("GL version: %s\n", glGetString(GL_VERSION));
	PrintGLErrors();
	if (!(program = LinkProgramViaCode(&vertexShader, &pixelShader)))
		getchar();
	InitVertexBuffer();
	glfwSetKeyCallback(window, Keyboard);
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

www.knhrowos.org