// header files
#include <gl/freeglut.h>
#include <math.h>
#define DEGREETORADIAN 0.0174533

// global variables
bool bFullScreen = false;
int iCount = 1;

// entry-point functions
int main(int argc, char* argv[], char* envp[]) {

	// local function declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void _display(int);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	// code
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(700, 600);
	glutInitWindowPosition(410, 130);
	glutCreateWindow("Hrushikesh Ahire : Emojis");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutIdleFunc(display);
	glutTimerFunc(3000, _display, NULL);

	glutMainLoop();

	return 0;
}

void _display(int x) {

	iCount++;
	glutTimerFunc(3000, _display, NULL);
}

void initialize(void) {

	// code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height) {

	// code
	glViewport(0, 0, width, height);
}

void display(void) {

	// variable declarations
	float f_angle;
	float x, y1, y2, y3, y4, y5;
	float f_x, f_y;
	float f_a, f_b;
	float f_radius_1 = 0.25f;
	float f_radius_2 = 0.25f;
	float f_gradient = 0.001f;
	float f_eye_size;

	// code
	glClear(GL_COLOR_BUFFER_BIT);

	// face
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 0.0f);
	for (f_angle = 0.0f; f_angle <= 360.0f; f_angle = f_angle + 1.0f) {
		glVertex3f((0.5f * cos(f_angle * DEGREETORADIAN)), (0.5f * sin(f_angle * DEGREETORADIAN)), 0.0f);
	}
	glEnd();

	switch (iCount) {
	case 1:
		// eyes
		glPointSize(25.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.17f, 0.17f, 0.0f);
		glVertex3f(0.17f, 0.17f, 0.0f);
		glEnd();

		// smile
		glPointSize(8.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		for (f_angle = 225.0f; f_angle <= 315.0f; f_angle = f_angle + 1.0f) {
			glVertex3f((0.25f * cos(f_angle * DEGREETORADIAN)), (0.25f * sin(f_angle * DEGREETORADIAN)), 0.0f);
		}
		glEnd();
	
		break;
	case 2:
		// eyes
		glPointSize(25.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.17f, 0.17f, 0.0f);
		glVertex3f(0.17f, 0.17f, 0.0f);
		glEnd();

		// big smile
		glPointSize(3.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		for (x = -0.19f; x <= 0.19f; x = x + 0.001f) {
			y1 = sqrt((4 - 25 * x * x) / (400));
			glVertex3f(x, -y1, 0.0f);

			if (x >= -0.18f && x <= 0.18f) {

				y3 = sqrt((4 - 25 * x * x) / (400)) + (0.06f);
				glVertex3f(x, -y3, 0.0f);
			}
		}

		for (x = -0.195f; x <= 0.195f; x = x + 0.001f) {
			y2 = sqrt((1369 - 34225 * x * x) / (10000));
			glVertex3f(x, -y2, 0.0f);
		}

		for (x = -0.095f; x <= 0.095; x = x + 0.001f) {

			y4 = (sqrt(0.02 - x * x)) - 0.42f;
			glVertex3f(x, y4, 0.0f);
		}

		glEnd();

		glPointSize(14.0f);
		//glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (x = -0.168f; x <= 0.168f; x = x + 0.005f) {
			y5 = sqrt((4 - 25 * x * x) / (400)) + (0.03f);
			glVertex3f(x, -y5, 0.0f);
		}

		glEnd();
		break;
	case 3:
		f_eye_size = 7.5f;
		glPointSize(f_eye_size);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		// left eye
		for (f_angle = 0.0f; f_angle <= 180.0f; f_angle = f_angle + 1.0f) {
			glVertex2f((-0.16f + (0.035f) * cos(f_angle * DEGREETORADIAN)), (0.16f + (0.035f) * sin(f_angle * DEGREETORADIAN)));
			if (f_angle <= 90.0f)
				f_eye_size = f_eye_size + 0.01f;
			else
				f_eye_size = f_eye_size - 0.01f;

			glPointSize(f_eye_size);
		}

		glEnd();

		// right eye
		f_eye_size = 7.5f;
		glPointSize(f_eye_size);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);

		for (f_angle = 0.0f; f_angle <= 180.0f; f_angle = f_angle + 1.0f) {
			glVertex2f((0.16f + (0.035f) * cos(f_angle * DEGREETORADIAN)), (0.16f + (0.035f) * sin(f_angle * DEGREETORADIAN)));
			if (f_angle <= 90.0f)
				f_eye_size = f_eye_size + 0.01f;
			else
				f_eye_size = f_eye_size - 0.01f;

			glPointSize(f_eye_size);
		}

		glEnd();

		// big smile
		glPointSize(3.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		for (x = -0.19f; x <= 0.19f; x = x + 0.001f) {
			y1 = sqrt((4 - 25 * x * x) / (400));
			glVertex3f(x, -y1, 0.0f);

			if (x >= -0.18f && x <= 0.18f) {

				y3 = sqrt((4 - 25 * x * x) / (400)) + (0.06f);
				glVertex3f(x, -y3, 0.0f);
			}
		}

		for (x = -0.195f; x <= 0.195f; x = x + 0.001f) {
			y2 = sqrt((1369 - 34225 * x * x) / (10000));
			glVertex3f(x, -y2, 0.0f);
		}

		// tongue
		for (x = -0.095f; x <= 0.095; x = x + 0.001f) {

			y4 = (sqrt(0.02 - x * x)) - 0.42f;
			glVertex3f(x, y4, 0.0f);
		}

		glEnd();

		// teeth
		glPointSize(14.0f);
		//glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (x = -0.168f; x <= 0.168f; x = x + 0.005f) {

			y5 = sqrt((4 - 25 * x * x) / (400)) + (0.03f);
			glVertex3f(x, -y5, 0.0f);
		}

		glEnd();
		break;
	case 4:
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(8.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);

		// left eye
		glVertex2f(-0.12f, 0.14f);
		glVertex2f(-0.20f, 0.08f);

		glVertex2f(-0.115f, 0.14f);
		glVertex2f(-0.20f, 0.20f);

		// right eye
		glVertex2f(0.12f, 0.14f);
		glVertex2f(0.20f, 0.08f);

		glVertex2f(0.115f, 0.14f);
		glVertex2f(0.20f, 0.20f);

		glEnd();

		// big smile
		glPointSize(3.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		for (x = -0.195f; x <= 0.195f; x = x + 0.001f) {
			y1 = sqrt((144 - 3600 * x * x) / (10000)) + (0.035f);
			glVertex3f(x, -y1, 0.0f);

			if (x >= -0.18f && x <= 0.18f) {

				y3 = sqrt((144 - 3600 * x * x) / (10000)) + (0.095f);
				glVertex3f(x, -y3, 0.0f);
			}
		}

		for (x = -0.197f; x <= 0.197f; x = x + 0.001f) {
			y2 = sqrt((1369 - 34225 * x * x) / (10000));
			glVertex3f(x, -y2, 0.0f);
		}

		// tongue
		for (x = -0.095f; x <= 0.095; x = x + 0.001f) {

			y4 = (sqrt(0.02 - x * x)) - 0.42f;
			glVertex3f(x, y4, 0.0f);
		}

		glEnd();

		// teeth
		glPointSize(5.0f);
		//glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (x = -0.185f; x <= 0.185f; x = x + 0.005f) {
			y1 = x;

			x = x + 0.0006F;
			y5 = sqrt((144 - 3600 * x * x) / (10000)) + (0.047f);
			glVertex3f(x, -y5, 0.0f);
			x = x + 0.0006F;

			y5 = sqrt((144 - 3600 * x * x) / (10000)) + (0.055f);
			glVertex3f(x, -y5, 0.0f);
			x = x + 0.0006F;

			y5 = sqrt((144 - 3600 * x * x) / (10000)) + (0.065f);
			glVertex3f(x, -y5, 0.0f);
			x = x + 0.0006F;

			y5 = sqrt((144 - 3600 * x * x) / (10000)) + (0.075f);
			glVertex3f(x, -y5, 0.0f);
			x = x + 0.0006F;

			y5 = sqrt((144 - 3600 * x * x) / (10000)) + (0.080f);
			glVertex3f(x, -y5, 0.0f);

			x = y1;
		}

		glEnd();
		break;
	
	case 5:
		// left eye
		glPointSize(35.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.15f, 0.13f, 0.0f);

		glEnd();

		// eyebrow
		glPointSize(8.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		for (f_angle = 90.0f; f_angle <= 150.0f; f_angle = f_angle + 1.0f) {
			glVertex2f((-0.15f + 0.07f * cos(f_angle * DEGREETORADIAN)), (0.13f + 0.07f * sin(f_angle * DEGREETORADIAN)));
		}

		glEnd();

		// right eye
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(8.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(0.12f, 0.14f);
		glVertex2f(0.20f, 0.08f);

		glVertex2f(0.115f, 0.14f);
		glVertex2f(0.20f, 0.20f);

		glEnd();

		// smile
		glPointSize(7.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		f_a = 0.18f;
		for (f_b = 0.1f; f_b <= 0.15f; f_b = f_b + 0.01f) {

			for (f_x = -0.17f; f_x <= 0.17f; f_x = f_x + 0.005f) {

				f_y = sqrt((f_b * f_b) * (1 - ((f_x * f_x) / (f_a * f_a)))) + 0.075f;
				glVertex2f(f_x, -f_y);
			}
		}

		glEnd();
		break;
	case 6:
		// left eye
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(8.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(-0.12f, 0.14f);
		glVertex2f(-0.20f, 0.08f);

		glVertex2f(-0.115f, 0.14f);
		glVertex2f(-0.20f, 0.20f);

		glEnd();


		// right eye
		glPointSize(15.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (f_angle = 0.0f; f_angle <= 360.0f; f_angle = f_angle + 1.0f) {
			glVertex3f((0.15f + 0.0275f * cos(f_angle * DEGREETORADIAN)), (0.13f + 0.0275f * sin(f_angle * DEGREETORADIAN)), 0.0f);
		}

		glEnd();

		glPointSize(15.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.15f, 0.13f, 0.0f);

		glEnd();

		// smile
		glPointSize(7.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		f_a = 0.19f;
		for (f_b = 0.075f; f_b <= 0.175f; f_b = f_b + 0.01f) {

			for (f_x = -0.18f; f_x <= 0.18f; f_x = f_x + 0.005f) {

				f_y = sqrt((f_b * f_b) * (1 - ((f_x * f_x) / (f_a * f_a)))) + 0.05f;
				glVertex2f(f_x, -f_y);
			}
		}

		glEnd();

		// tongue
		glPointSize(3.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.819607f, 0.564705f, 0.556862f);

		f_a = 0.07f;
		for (f_b = 0.0f; f_b <= 0.2f; f_b = f_b + 0.005f) {

			for (f_x = -0.045f; f_x <= 0.045f; f_x = f_x + 0.001f) {

				f_y = sqrt((f_b * f_b) * (1 - ((f_x * f_x) / (f_a * f_a)))) + 0.145f;
				glVertex2f(f_x, -f_y);
			}
		}

		glEnd();
		break;
	case 7:
		// eyes
		glPointSize(25.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.17f, 0.17f, 0.0f);
		glVertex3f(0.17f, 0.17f, 0.0f);
		glEnd();

		// sad smile
		glPointSize(3.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		f_a = 0.07f;
		for (f_b = 0.11f; f_b >= 0.0f; f_b = f_b - 0.002f) {

			for (f_x = -0.17f; f_x <= 0.17f; f_x = f_x + 0.002f) {

				f_y = sqrt((f_b * f_b) * (1 - ((f_x * f_x) / (f_a * f_a)))) - 0.2f;
				glVertex2f(f_x, f_y);
				glVertex2f(f_x, -(f_y + 0.4f));
			}
		}

		glEnd();
		break;
	case 8:
		// eyes
		glPointSize(25.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.17f, 0.17f, 0.0f);
		glVertex3f(0.17f, 0.17f, 0.0f);
		glEnd();

		// sad smile
		glPointSize(3.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);

		f_a = 0.18f;
		for (f_b = 0.15f; f_b >= 0.1f; f_b = f_b - 0.002f) {

			for (f_x = -0.17f; f_x <= 0.17f; f_x = f_x + 0.002f) {

				f_y = sqrt((f_b * f_b) * (1 - ((f_x * f_x) / (f_a * f_a)))) - 0.225f;
				glVertex2f(f_x, f_y);
			}
		}

		glEnd();
		break;
	case 9:
		// eyes
		glPointSize(25.0f);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.17f, 0.17f, 0.0f);
		glVertex3f(0.17f, 0.17f, 0.0f);
		glEnd();

		// smile
		glLineWidth(8.0f);
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);

		glVertex2f(-0.17f, -0.15f);
		glVertex2f(0.17f, -0.15f);

		glEnd();
		break;
	case 10:
		// eyes
		glLineWidth(7.5f);
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);

		// left eye
		glVertex2f(-0.21f, 0.15f);
		glVertex2f(-0.13f, 0.15f);

		// right eye
		glVertex2f(0.21f, 0.15f);
		glVertex2f(0.13f, 0.15f);

		glEnd();

		// smile
		glLineWidth(8.0f);
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);

		glVertex2f(-0.17f, -0.15f);
		glVertex2f(0.17f, -0.15f);

		glEnd();
		break;
	default:
		exit(0);
		break;
	}
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {

	// code
	switch (key) {
	case 27:	// ascii value of escape key
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bFullScreen == false) {

			glutFullScreen();
			bFullScreen = true;
		}
		else {

			glutLeaveFullScreen();
			bFullScreen = false;
		}
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y) {

	// code
	switch (button) {
	case GLUT_LEFT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void uninitialize(void) {

	// code
}

