// Emoji Expresser Team: display Emoji face on a background image
// the emoji changes in response to user action
// 3 emojis: Smiley, Valentine, Sarcastic
// each with 5 outlines: 1 lips, 2 eyes, 2 eyebrows
// 4 actions: Poke, Love, Slap, Pat
// 5 results:
// Smiley [Poke] --> Sarcastic
// Smiley [Love] --> Valentine
// Valentine [Slap] --> Sarcastic
// Sarcastic [Pat] --> Smiley
// Sarcastic [Love] --> Valentine

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

// Application
bool DIAGNOSTICS = false;
int winWidth = 700, winHeight = 700;
bool wireframe = false, numberVertices = false, numberTriangles = false;

// Sprites
Sprite background, slapButton;
Sprite* buttons[] = { &slapButton };
int nbuttons = sizeof(buttons) / sizeof(Sprite*);

// Filenames
string dir = "C:/Users/mosta/Graphics/Apps/";
string backgroundTex = "emoji.tga";
string slapButtonTex = "slapEmoji.jpg";

// Circle Outline
const int nOutlinePts = 40;
vec2 circle[nOutlinePts];

// Valentine Outlines
vec2 valentineMouth[] = {
{-0.49, -0.33}, {-0.53, -0.28}, {-0.56, -0.23}, {-0.58, -0.17}, {-0.56, -0.15}, {-0.53, -0.17}, {-0.50, -0.19}, {-0.45, -0.22},
{-0.38, -0.25}, {-0.29, -0.28}, {-0.22, -0.29}, {-0.14, -0.31}, {-0.06, -0.32}, {0.01, -0.32}, {0.09, -0.31}, {0.17, -0.30},
{0.25, -0.28}, {0.33, -0.26}, {0.40, -0.23}, {0.46, -0.20}, {0.50, -0.16}, {0.52, -0.16}, {0.53, -0.18}, {0.51, -0.22},
{0.49, -0.28}, {0.44, -0.34}, {0.38, -0.40}, {0.30, -0.46}, {0.22, -0.50}, {0.16, -0.52}, {0.09, -0.54}, {-0.01, -0.55},
{-0.07, -0.55}, {-0.13, -0.54}, {-0.21, -0.52}, {-0.27, -0.50}, {-0.33, -0.46}, {-0.38, -0.43}, {-0.42, -0.40}, {-0.46, -0.37}
};
vec2 valentineLEye[] = { //corrected
{0.34, -0.87}, {0.24, -0.84}, {0.15, -0.81}, {0.05, -0.76}, {-0.03, -0.72}, {-0.12, -0.65}, {-0.20, -0.60}, {-0.30, -0.53},
{-0.40, -0.45}, {-0.47, -0.38}, {-0.54, -0.29}, {-0.61, -0.21}, {-0.65, -0.13}, {-0.67, -0.04}, {-0.67, 0.05}, {-0.63, 0.12},
{-0.58, 0.19}, {-0.50, 0.26}, {-0.42, 0.30}, {-0.33, 0.32}, {-0.22, 0.31}, {-0.09, 0.28}, {0.01, 0.23}, {0.12, 0.18},
{0.18, 0.25}, {0.22, 0.30}, {0.29, 0.39}, {0.41, 0.44}, {0.55, 0.48}, {0.71, 0.47}, {0.82, 0.41}, {0.91, 0.32},
{0.95, 0.18}, {0.93, 0.01}, {0.86, -0.19}, {0.79, -0.33}, {0.71, -0.46}, {0.63, -0.58}, {0.54, -0.69}, {0.44, -0.81}

};
vec2 valentineREye[] = { //corrected
{-0.32, -0.77}, {-0.41, -0.70}, {-0.49, -0.62}, {-0.55, -0.54}, {-0.63, -0.44}, {-0.68, -0.36}, {-0.74, -0.26}, {-0.80, -0.17},
{-0.84, -0.07}, {-0.88, 0.03}, {-0.89, 0.14}, {-0.87, 0.24}, {-0.82, 0.32}, {-0.75, 0.38}, {-0.66, 0.43}, {-0.55, 0.43},
{-0.43, 0.43}, {-0.33, 0.40}, {-0.24, 0.34}, {-0.16, 0.26}, {-0.07, 0.18}, {0.02, 0.20}, {0.15, 0.26}, {0.28, 0.29},
{0.40, 0.29}, {0.49, 0.26}, {0.57, 0.22}, {0.65, 0.13}, {0.70, 0.01}, {0.68, -0.09}, {0.64, -0.18}, {0.56, -0.28},
{0.50, -0.34}, {0.42, -0.40}, {0.35, -0.46}, {0.26, -0.52}, {0.18, -0.57}, {0.07, -0.63}, {-0.03, -0.69}, {-0.19, -0.75}
};
vec2 valentineLEyebrow[] = { // clockwise
{-0.47, 0.46}, {-0.45, 0.52}, {-0.42, 0.56}, {-0.39, 0.60}, {-0.36, 0.63}, {-0.32, 0.67}, {-0.29, 0.70}, {-0.25, 0.73},
{-0.21, 0.75}, {-0.16, 0.76}, {-0.12, 0.78}, {-0.08, 0.79}, {-0.03, 0.79}, {0.01, 0.79}, {0.04, 0.79}, {0.08, 0.78},
{0.11, 0.77}, {0.15, 0.76}, {0.18, 0.74}, {0.21, 0.71}, {0.20, 0.67}, {0.16, 0.65}, {0.13, 0.67}, {0.09, 0.68},
{0.06, 0.69}, {0.03, 0.69}, {0.00, 0.70}, {-0.03, 0.70}, {-0.08, 0.69}, {-0.11, 0.69}, {-0.16, 0.67}, {-0.20, 0.65},
{-0.22, 0.63}, {-0.25, 0.61}, {-0.28, 0.58}, {-0.31, 0.55}, {-0.34, 0.51}, {-0.38, 0.46}, {-0.40, 0.41}, {-0.45, 0.42}
};
vec2 valentineREyebrow[] = { // clockwise
{-0.20, 0.65}, {-0.16, 0.67}, {-0.13, 0.69}, {-0.08, 0.70}, {-0.04, 0.71}, {0.00, 0.71}, {0.05, 0.72}, {0.09, 0.71},
{0.14, 0.71}, {0.19, 0.69}, {0.23, 0.67}, {0.27, 0.65}, {0.32, 0.62}, {0.36, 0.59}, {0.41, 0.54}, {0.44, 0.49},
{0.47, 0.45}, {0.49, 0.41}, {0.46, 0.37}, {0.42, 0.38}, {0.38, 0.42}, {0.35, 0.46}, {0.32, 0.50}, {0.28, 0.53},
{0.25, 0.56}, {0.21, 0.58}, {0.17, 0.60}, {0.14, 0.61}, {0.11, 0.62}, {0.07, 0.62}, {0.04, 0.63}, {0.00, 0.63},
{-0.02, 0.63}, {-0.05, 0.62}, {-0.08, 0.61}, {-0.10, 0.60}, {-0.13, 0.60}, {-0.15, 0.60}, {-0.17, 0.60}, {-0.19, 0.62}
};

// Smiley Outlines
vec2 smileyMouth[] = { // clockwise
{-0.66, -0.03}, {-0.61, -0.07}, {-0.56, -0.11}, {-0.50, -0.15}, {-0.45, -0.18}, {-0.37, -0.22}, {-0.31, -0.25}, {-0.23, -0.27},
{-0.16, -0.29}, {-0.08, -0.30}, {-0.00, -0.30}, {0.08, -0.29}, {0.16, -0.28}, {0.23, -0.26}, {0.30, -0.24}, {0.36, -0.21},
{0.41, -0.18}, {0.46, -0.16}, {0.51, -0.13}, {0.57, -0.09}, {0.64, -0.04}, {0.58, -0.15}, {0.53, -0.22}, {0.48, -0.28},
{0.44, -0.33}, {0.40, -0.38}, {0.35, -0.43}, {0.28, -0.49}, {0.22, -0.55}, {0.12, -0.61}, {0.03, -0.64}, {-0.07, -0.65},
{-0.16, -0.61}, {-0.25, -0.54}, {-0.32, -0.48}, {-0.38, -0.41}, {-0.45, -0.34}, {-0.51, -0.26}, {-0.56, -0.19}, {-0.62, -0.11}
};
vec2 smileyLEye[] = { // clockwise
{-0.41, 0.53}, {-0.38, 0.55}, {-0.36, 0.55}, {-0.33, 0.55}, {-0.31, 0.54}, {-0.29, 0.52}, {-0.27, 0.51}, {-0.26, 0.48},
{-0.24, 0.46}, {-0.23, 0.44}, {-0.22, 0.42}, {-0.22, 0.39}, {-0.21, 0.36}, {-0.21, 0.34}, {-0.20, 0.32}, {-0.20, 0.29},
{-0.21, 0.25}, {-0.21, 0.22}, {-0.22, 0.19}, {-0.23, 0.16}, {-0.24, 0.14}, {-0.25, 0.12}, {-0.27, 0.10}, {-0.29, 0.09},
{-0.32, 0.08}, {-0.34, 0.08}, {-0.37, 0.08}, {-0.39, 0.10}, {-0.41, 0.12}, {-0.44, 0.15}, {-0.45, 0.17}, {-0.47, 0.21},
{-0.48, 0.25}, {-0.48, 0.29}, {-0.49, 0.34}, {-0.48, 0.38}, {-0.47, 0.43}, {-0.46, 0.47}, {-0.45, 0.49}, {-0.43, 0.52}
};
vec2 smileyREye[] = { // clockwise
{0.37, 0.54}, {0.39, 0.53}, {0.41, 0.51}, {0.42, 0.49}, {0.43, 0.47}, {0.44, 0.44}, {0.45, 0.41}, {0.46, 0.37},
{0.46, 0.34}, {0.46, 0.30}, {0.45, 0.26}, {0.44, 0.22}, {0.43, 0.18}, {0.41, 0.15}, {0.39, 0.12}, {0.37, 0.10},
{0.35, 0.09}, {0.33, 0.08}, {0.31, 0.08}, {0.28, 0.08}, {0.25, 0.10}, {0.23, 0.12}, {0.22, 0.14}, {0.20, 0.17},
{0.19, 0.20}, {0.18, 0.24}, {0.18, 0.28}, {0.18, 0.32}, {0.18, 0.36}, {0.19, 0.39}, {0.20, 0.43}, {0.21, 0.45},
{0.23, 0.48}, {0.24, 0.50}, {0.26, 0.52}, {0.27, 0.53}, {0.29, 0.54}, {0.32, 0.55}, {0.34, 0.55}, {0.36, 0.55}
};
vec2 smileyLEyebrow[] = { // corrected clockwise
{-0.61, 0.59},{-0.60, 0.60},{-0.58, 0.62},{-0.56, 0.63},{-0.54, 0.64},{-0.52, 0.65},{-0.50, 0.66},{-0.47, 0.67},
{-0.45, 0.68},{-0.43, 0.69},{-0.41, 0.69},{-0.38, 0.70},{-0.37, 0.70},{-0.34, 0.70},{-0.32, 0.70},{-0.31, 0.70},
{-0.29, 0.70},{-0.28, 0.69},{-0.26, 0.69},{-0.24, 0.68},{-0.26, 0.67},{-0.28, 0.67},{-0.29, 0.68},{-0.31, 0.68},
{-0.33, 0.67},{-0.34, 0.67},{-0.36, 0.67},{-0.38, 0.66},{-0.40, 0.65},{-0.42, 0.65},{-0.44, 0.64},{-0.47, 0.62},
{-0.48, 0.62}, {-0.51, 0.61},{-0.52, 0.60},{-0.54, 0.59},{-0.56, 0.58},{-0.57, 0.57},{-0.59, 0.56},{-0.60, 0.57}
};
vec2 smileyREyebrow[] = { //corrected clockwise
{0.21, 0.68},{0.23, 0.69},{0.24, 0.70},{0.26, 0.70},{0.29, 0.70},{0.31, 0.70},{0.33, 0.70},{0.35, 0.69},{0.38, 0.69},
{0.40, 0.69},{0.42, 0.68},{0.44, 0.67},{0.46, 0.66},{0.48, 0.66},{0.50, 0.65},{0.52, 0.64},{0.54, 0.63},{0.55, 0.62},
{0.57, 0.60},{0.59, 0.59},{0.57, 0.57},{0.56, 0.56},{0.54, 0.57},{0.52, 0.59},{0.50, 0.60},{0.48, 0.61},{0.46, 0.62},
{0.44, 0.63},{0.41, 0.64},{0.40, 0.65},{0.38, 0.65},{0.36, 0.66},{0.33, 0.66},{0.32, 0.66},{0.31, 0.67},{0.29, 0.67},
{0.28, 0.67},{0.27, 0.67},{0.25, 0.67},{0.24, 0.67}
};

// Sarcastic Outlines
vec2 sarcasticMouth[] = { // clockwise
{-0.24, -0.40}, {-0.21, -0.38}, {-0.19, -0.38}, {-0.16, -0.38}, {-0.13, -0.38}, {-0.09, -0.37}, {-0.06, -0.38}, {-0.02, -0.37},
{0.01, -0.36}, {0.04, -0.36}, {0.07, -0.35}, {0.10, -0.33}, {0.12, -0.32}, {0.15, -0.31}, {0.17, -0.29}, {0.19, -0.28},
{0.21, -0.27}, {0.24, -0.25}, {0.27, -0.24}, {0.30, -0.24}, {0.31, -0.26}, {0.30, -0.29}, {0.29, -0.31}, {0.27, -0.34},
{0.24, -0.37}, {0.21, -0.39}, {0.18, -0.42}, {0.13, -0.42}, {0.10, -0.43}, {0.07, -0.43}, {0.04, -0.43}, {0.02, -0.43},
{-0.02, -0.43}, {-0.05, -0.43}, {-0.07, -0.43}, {-0.10, -0.43}, {-0.12, -0.43}, {-0.15, -0.43}, {-0.19, -0.43}, {-0.23, -0.42}
};
vec2 sarcasticEye[] = { // clockwise
{-0.48, 0.05}, {-0.46, 0.07}, {-0.43, 0.09}, {-0.40, 0.11}, {-0.37, 0.12}, {-0.34, 0.13}, {-0.31, 0.13}, {-0.28, 0.13},
{-0.24, 0.14}, {-0.21, 0.14}, {-0.18, 0.13}, {-0.16, 0.13}, {-0.13, 0.13}, {-0.11, 0.12}, {-0.09, 0.11}, {-0.08, 0.08},
{-0.08, 0.03}, {-0.09, -0.01}, {-0.10, -0.04}, {-0.12, -0.05}, {-0.14, -0.06}, {-0.17, -0.07}, {-0.19, -0.07}, {-0.21, -0.06},
{-0.23, -0.04}, {-0.25, -0.02}, {-0.26, -0.00}, {-0.26, 0.03}, {-0.27, 0.03}, {-0.29, 0.03}, {-0.32, 0.03}, {-0.34, 0.02},
{-0.36, 0.01}, {-0.38, 0.01}, {-0.40, -0.00}, {-0.41, -0.01}, {-0.43, -0.02}, {-0.46, -0.02}, {-0.48, -0.01}, {-0.49, 0.02}

};

vec2 sarcasticLEyebrow[] = { // clockwise
{-0.61, 0.20}, {-0.59, 0.23}, {-0.57, 0.26}, {-0.55, 0.28}, {-0.52, 0.31}, {-0.49, 0.33}, {-0.47, 0.35}, {-0.44, 0.36},
{-0.41, 0.37}, {-0.38, 0.39}, {-0.35, 0.40}, {-0.31, 0.41}, {-0.28, 0.41}, {-0.25, 0.41}, {-0.22, 0.38}, {-0.22, 0.34},
{-0.24, 0.32}, {-0.26, 0.32}, {-0.29, 0.31}, {-0.30, 0.31}, {-0.32, 0.31}, {-0.33, 0.30}, {-0.35, 0.30}, {-0.38, 0.29},
{-0.40, 0.28}, {-0.42, 0.27}, {-0.44, 0.25}, {-0.46, 0.24}, {-0.47, 0.22}, {-0.49, 0.21}, {-0.50, 0.20}, {-0.51, 0.18},
{-0.52, 0.17}, {-0.54, 0.15}, {-0.55, 0.14}, {-0.57, 0.13}, {-0.59, 0.13}, {-0.60, 0.13}, {-0.61, 0.14}, {-0.62, 0.18}
};
vec2 sarcasticREyebrow[] = { // clockwise
{0.24, 0.38}, {0.25, 0.40}, {0.27, 0.41}, {0.29, 0.41}, {0.32, 0.41}, {0.35, 0.40}, {0.37, 0.40}, {0.39, 0.39},
{0.41, 0.39}, {0.43, 0.37}, {0.46, 0.36}, {0.48, 0.34}, {0.50, 0.33}, {0.52, 0.32}, {0.55, 0.30}, {0.57, 0.27},
{0.60, 0.25}, {0.62, 0.22}, {0.64, 0.19}, {0.65, 0.16}, {0.63, 0.14}, {0.62, 0.13}, {0.60, 0.12}, {0.58, 0.13},
{0.56, 0.15}, {0.54, 0.18}, {0.51, 0.21}, {0.49, 0.23}, {0.47, 0.25}, {0.45, 0.26}, {0.41, 0.28}, {0.39, 0.29},
{0.37, 0.29}, {0.34, 0.31}, {0.32, 0.31}, {0.30, 0.31}, {0.28, 0.31}, {0.26, 0.32}, {0.24, 0.33}, {0.24, 0.36}
};

// Animatable Outline
class Outline {
public:
	string name;
	vector<vec2> points;
	vector<int3> triangles;
	GLuint vBuffer = 0, program = 0;
	vec3 color;
	vec2 location;
	float scale = 1;
	string NameFromContour(vec2* pts) {
		return pts == circle ? "circle" :
			pts == valentineMouth ? "valentineMouth" :
			pts == valentineLEye ? "valentineLEye" :
			pts == valentineREye ? "valentineREye" :
			pts == valentineLEyebrow ? "valentineLEyebrow" :
			pts == valentineREyebrow ? "valentineREyebrow" :
			pts == smileyMouth ? "smileyMouth" :
			pts == smileyLEye ? "smileyLEye" :
			pts == smileyREye ? "smileyREye" :
			pts == smileyLEyebrow ? "smileyLEyebrow" :
			pts == smileyREyebrow ? "smileyREyebrow" :
			pts == sarcasticMouth ? "sarcasticMouth" :
			pts == sarcasticEye ? "sarcasticEye" :
			pts == sarcasticLEyebrow ? "sarcasticLEyebrow" :
			pts == sarcasticREyebrow ? "sarcasticREyebrow" : "unknown";
	}
	void Initialize(const char* namePtr, Outline& o) {
		Initialize(o.points.data(), o.points.size(), o.location, o.scale, o.color, namePtr);
	}
	void Initialize(vec2* pts, int npts, vec2 loc, float s, vec3 col, const char* namePtr = NULL) {
		name = namePtr ? string(namePtr) : NameFromContour(pts);
		location = loc;
		scale = s;
		color = col;
		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferData(GL_ARRAY_BUFFER, npts * sizeof(vec2), NULL, GL_STATIC_DRAW);
		points.resize(npts);
		for (int i = 0; i < npts; i++)
			points[i] = pts[i];
		UpdateVertices();
		Triangulate();
	}
	void UpdateVertices() {
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(vec2), points.data());
	}
	mat4 GetMatrix() { return Translate(location.x, location.y, 0) * Scale(scale, scale, 1); }
	bool IsZero(float f) { return abs(f) < FLT_EPSILON; }
	vec3 LineBetweenPoints(vec2& p1, vec2& p2) {
		float x = p1.y - p2.y, y = p2.x - p1.x, z = p1.x * p2.y - p2.x * p1.y;
		float len = sqrt(x * x + y * y);
		return IsZero(len) ? vec3(x, y, z) : vec3(x, y, z) / len;
	}
	bool SegmentCrossLine(vec2& seg1, vec2& seg2, vec2& line1, vec2& line2) {
		vec3 line = LineBetweenPoints(line1, line2);
		float d1a = seg1.x * line.x + seg1.y * line.y + line.z;
		float d1b = seg2.x * line.x + seg2.y * line.y + line.z;
		return (d1a > 0) != (d1b > 0);
	}
	bool IntersectTwoSegments(vec2& s1a, vec2& s1b, vec2& s2a, vec2& s2b) {
		return SegmentCrossLine(s1a, s1b, s2a, s2b) && SegmentCrossLine(s2a, s2b, s1a, s1b);
	}
	bool IntersectTwo(int ia, int ib, int i1, int i2) {
		bool connected = i1 == ia || i1 == ib || i2 == ia || i2 == ib;
		return !connected && IntersectTwoSegments(points[ia], points[ib], points[i1], points[i2]);
	}
	bool SelfIntersect(int ia, int ib, vector<int>& ids) {
		// does edge ia,ib intersect a remaining contour edge?
		int nids = ids.size();
		for (int i = 0; i < nids; i++)
			if (IntersectTwo(ia, ib, ids[i], ids[(i + 1) % nids]))
				return true;
		// does edge ia,ib intersect with a triangle edge?
		for (int i = 0; i < (int)triangles.size(); i++) {
			int3 t = triangles[i];
			if (IntersectTwo(ia, ib, t.i1, t.i2)) return true;
			if (IntersectTwo(ia, ib, t.i2, t.i3)) return true;
			if (IntersectTwo(ia, ib, t.i3, t.i1)) return true;
		}
		return false;
	}
	int MostAcute(vector<int>& ids) {
		float minDot = 1;
		int nids = ids.size(), minI = -1;
		for (int i = 0; i < nids; i++) {
			int i0 = ids[i ? i - 1 : nids - 1], i1 = ids[i], i2 = ids[(i + 1) % nids];
			vec2 p0 = points[i0], p1 = points[i1], p2 = points[i2];
			vec2 v1 = normalize(p1 - p0), v2 = normalize(p2 - p1);
			float d = dot(v1, v2);
			bool selfIntersect = SelfIntersect(i0, i2, ids);
			if (d < minDot && cross(v1, v2) < 0 && !selfIntersect) { minDot = d; minI = i; } // clockwise
			// if (d > maxDot && cross(v1, v2) < 0 && !selfIntersect) { maxDot = d; maxI = i; } // counter-clockwise
		}
		return minI;
	}
	void Triangulate() {
		vector<int> ids(points.size()); // remaining vertices to be triangulated
		for (int i = 0; i < (int)points.size(); i++)
			ids[i] = i;
		triangles.resize(0);
		for (;;) {
			int nids = ids.size();
			if (nids == 3)
				triangles.push_back(int3(ids[0], ids[1], ids[2]));
			if (nids < 4)
				return;
			int i = MostAcute(ids);
			if (i == -1) {
				printf("%s: MostAcute error\n", name.c_str());
				return;
			}
			int i0 = ids[i ? i - 1 : nids - 1], i1 = ids[i], i2 = ids[(i + 1) % nids];
			triangles.push_back(int3(i0, i1, i2));
			ids.erase(ids.begin() + i);
		}
	}
	void Interpolate(Outline& o1, Outline& o2, float a) {
		for (int i = 0; i < (int)points.size(); i++)
			points[i] = o1.points[i] + a * (o2.points[i] - o1.points[i]);
		location = o1.location + a * (o2.location - o1.location);
		scale = o1.scale + a * (o2.scale - o1.scale);
		color = o1.color + a * (o2.color - o1.color);
		Triangulate();
		UpdateVertices();
	}
	void Display() {
		const char* vertexShader = R"(
#version 130
in vec2 point;
uniform mat4 m = mat4(1);
void main() { gl_Position = m*vec4(point, 0, 1); }
)";
		const char* pixelShader = R"(
#version 130
uniform vec3 color;
out vec4 pColor;
void main() { pColor = vec4(color, 1); }
)";
		mat4 m = GetMatrix();
		if (!program)
			program = LinkProgramViaCode(&vertexShader, &pixelShader);
		glUseProgram(program);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
		VertexAttribPointer(program, "point", 2, 0, (void*)0);
		SetUniform(program, "color", color);
		SetUniform(program, "m", m);
		if (!DIAGNOSTICS) {
			if (wireframe) {
				glLineWidth(1);
				for (int i = 0; i < (int)triangles.size(); i++)
					glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, &triangles[i]);
				glPointSize(9);
				glDrawElements(GL_POINTS, 3 * triangles.size(), GL_UNSIGNED_INT, triangles.data());
			}
			else
				glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, triangles.data());
		}
		if (numberVertices || DIAGNOSTICS)
			for (int i = 0; i < (int)points.size(); i++) {
				vec2 s = ScreenPoint(vec3(points[i], 0), m);
				Number((int)s.x, (int)s.y, i);
			}
		if (numberTriangles)
			for (int i = 0; i < (int)triangles.size(); i++) {
				int3& t = triangles[i];
				vec2 center((points[t.i1] + points[t.i2] + points[t.i3]) / 3), s = ScreenPoint(vec3(center, 0), m);
				Number((int)s.x, (int)s.y, i, vec3(1, 1, 0), 12);
			}
		if (DIAGNOSTICS) {
			UseDrawShader(m);
			int npoints = points.size();
			for (int i = 0; i < npoints; i++)
				Line(points[i], points[(i + 1) % npoints], 2, vec3(1, 0, 0));
		}
	}
	~Outline() { if (vBuffer > 0) glDeleteBuffers(1, &vBuffer); }
};

class Emoji {
public:
	Outline face, mouth, lEye, rEye, lEyebrow, rEyebrow;
	void Display() {
		if (!DIAGNOSTICS) {
			face.Display();
			mouth.Display();
			lEye.Display();
			rEye.Display();
		}
		lEyebrow.Display();
		rEyebrow.Display();
	}
	void Interpolate(Emoji* e1, Emoji* e2, float a) {
		face.Interpolate(e1->face, e2->face, a);
		mouth.Interpolate(e1->mouth, e2->mouth, a);
		lEye.Interpolate(e1->lEye, e2->lEye, a);
		rEye.Interpolate(e1->rEye, e2->rEye, a);
		lEyebrow.Interpolate(e1->lEyebrow, e2->lEyebrow, a);
		rEyebrow.Interpolate(e1->rEyebrow, e2->rEyebrow, a);
	}
} smileE, sarcasticE, valentineE, displayE;

enum EmojiType { ET_Smiley, ET_Sarcastic, ET_Valentine, ET_None };
const char* emojiTypeNames[] = { "Smiley", "Sarcastic", "Valentine", "None" };
EmojiType current = ET_Smiley, destination = ET_None;

Emoji* GetEmoji(EmojiType t) {
	return t == ET_Smiley ? &smileE : t == ET_Valentine ? &valentineE : t == ET_Sarcastic ? &sarcasticE : NULL;
}

// Initialization

void InitCircle(bool cw = true) {
	for (int i = 0; i < nOutlinePts; i++) {
		float t = (float)i / nOutlinePts, tt = cw ? 1 - t : t, a = 2 * 3.141592f * tt;
		circle[i] = vec2(cos(a), sin(a));
	}
}

void InitSmiley() {
	smileE.face.Initialize(circle, nOutlinePts, vec2(0, 0), 0.9f, vec3(1, 1, 0));
	smileE.mouth.Initialize(smileyMouth, nOutlinePts, vec2(0.0f, -.1f), .6f, vec3(.7f, 0, 0));
	smileE.lEye.Initialize(smileyLEye, nOutlinePts, vec2(-.1f, .1f), .6f, vec3(.7f, 0, 0));
	smileE.rEye.Initialize(smileyREye, nOutlinePts, vec2(.1f, .1f), .6f, vec3(.7f, 0, 0));
	if (DIAGNOSTICS) {
		smileE.lEyebrow.Initialize(smileyLEyebrow, nOutlinePts, vec2(-.21f, 0.f), .6f, vec3(.7f, 0, 0)); // diagnostic
		smileE.rEyebrow.Initialize(smileyREyebrow, nOutlinePts, vec2(.8f, 0.f), .6f, vec3(.7f, 0, 0)); // diagnostic
	}
	else {
		smileE.lEyebrow.Initialize(smileyLEyebrow, nOutlinePts, vec2(-.15f, .2f), .6f, vec3(.7f, 0, 0));
		smileE.rEyebrow.Initialize(smileyREyebrow, nOutlinePts, vec2(.15f, .2f), .6f, vec3(.7f, 0, 0));
	}
}

void InitValentine() {
	valentineE.face.Initialize(circle, nOutlinePts, vec2(0, 0), 0.9f, vec3(1, 1, 0));
	valentineE.mouth.Initialize(valentineMouth, nOutlinePts, vec2(0.0f, -.1f), .8f, vec3(.7f, 0, 0));
	// valentineE.lEye.Initialize(valentineLEye, nOutlinePts, vec2(2.4f,-.15f), 5, vec3(.7f, 0, 0)); // diagnostic
	valentineE.lEye.Initialize(valentineLEye, nOutlinePts, vec2(-.40f, .30f), .2f, vec3(.7f, 0, 0));
	valentineE.rEye.Initialize(valentineREye, nOutlinePts, vec2(.40f, .30f), .2f, vec3(.7f, 0, 0));
	valentineE.lEyebrow.Initialize(valentineLEyebrow, nOutlinePts, vec2(-.40f, .2f), .5f, vec3(.7f, 0, 0));
	valentineE.rEyebrow.Initialize(valentineREyebrow, nOutlinePts, vec2(.40f, .2f), .5f, vec3(.7f, 0, 0));
}

void InitSarcastic() {
	sarcasticE.face.Initialize(circle, nOutlinePts, vec2(0, 0), 0.9f, vec3(1, 1, 0));
	sarcasticE.mouth.Initialize(sarcasticMouth, nOutlinePts, vec2(.0f, -.1f), .9f, vec3(.7f, 0, 0));
	sarcasticE.lEye.Initialize(sarcasticEye, nOutlinePts, vec2(-.2f, .0f), .8f, vec3(.7f, 0, 0));
	sarcasticE.rEye.Initialize(sarcasticEye, nOutlinePts, vec2(.5f, .0f), .9f, vec3(.7f, 0, 0));
	sarcasticE.lEyebrow.Initialize(sarcasticLEyebrow, nOutlinePts, vec2(-.25f, .1f), .8f, vec3(.7f, 0, 0));
	sarcasticE.rEyebrow.Initialize(sarcasticREyebrow, nOutlinePts, vec2(.10f, .1f), .8f, vec3(.7f, 0, 0));
}

void InitDisplay() {
	Emoji* c = GetEmoji(current);
	displayE.face.Initialize(circle, nOutlinePts, vec2(0, 0), 0.9f, vec3(1, 1, 0));
	displayE.mouth.Initialize("displayMouth", c->mouth);
	displayE.lEye.Initialize("displayLEye", c->lEye);
	displayE.rEye.Initialize("displayREye", c->rEye);
	displayE.lEyebrow.Initialize("displayLEyebrow", c->lEyebrow);
	displayE.rEyebrow.Initialize("displayREyebrow", c->rEyebrow);
}

void InitShapes() {
	InitCircle();
	InitValentine();
	InitSarcastic();
	InitSmiley();
	InitDisplay();
}

// Gameplay

enum Action { A_Slap = 0, A_Pat, A_Poke, A_Love, A_None };
const char* actionNames[] = { "Slap", "Pat", "Poke", "Love", "None" };

bool animating = false;
float animationDuration = .5f;
time_t animationStart;

void NewAction(Action a) {
	if (current == ET_Smiley && a == A_Poke) destination = ET_Sarcastic;
	if (current == ET_Smiley && a == A_Love) destination = ET_Valentine;
	if (current == ET_Valentine && a == A_Slap) destination = ET_Sarcastic;
	if (current == ET_Sarcastic && a == A_Pat) destination = ET_Smiley;
	if (current == ET_Sarcastic && a == A_Love) destination = ET_Valentine;
	animating = destination != ET_None;
}

void Animate() {
	if (!animating)
		animationStart = clock();
	if (animating) {
		float dt = (float)(clock() - animationStart) / CLOCKS_PER_SEC;
		if (dt > animationDuration) {
			animating = false;
			current = destination;
			destination = ET_None;
			printf("now %s\n", emojiTypeNames[current]);
		}
		else
			if (destination != ET_None)
				displayE.Interpolate(GetEmoji(current), GetEmoji(destination), dt / animationDuration);
	}
}

// Display

void Display() {
	glClearColor(.7f, .7f, .7f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	if (!DIAGNOSTICS)
		background.Display();
	displayE.Display();
	slapButton.Display();
	glFlush();
}

// Interaction

void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool shift = mods & GLFW_MOD_SHIFT;
	if (action == GLFW_PRESS)
		switch (key) {
		case 'L': NewAction(A_Love); break;
		case 'P': NewAction(A_Pat); break;
		case 'K': NewAction(A_Poke); break;
		case 'S': NewAction(A_Slap); break;
		case 'W': wireframe = !wireframe; break;
		case 'V': numberVertices = !numberVertices; break;
		case 'T': numberTriangles = !numberTriangles; break;
		}
}

void MouseButton(GLFWwindow* w, int butn, int action, int mods) {
	double x, y;
	glfwGetCursorPos(w, &x, &y);
	y = winHeight - y; // invert y for upward-increasing screen space
	if (action == GLFW_PRESS) {
		for (int i = 0; i < nbuttons; i++)
			if (buttons[i]->Hit((int)x, (int)y))
				NewAction((Action)i);
	}
}

// Application

void Resize(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, winWidth = width, winHeight = height);
}

const char* usage = R"(
Smiley [K: Poke] --> Sarcastic
Smiley [L: Love] --> Valentine
Valentine [S: Slap] --> Sarcastic
Sarcastic [P: Pat] --> Smiley
Sarcastic [L: Love] --> Valentine
W: toggle wireframe
V: toggle vertex numbers
T: toggle triangle numbers
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
	background.Initialize(dir + backgroundTex, "", 0, .7f);
	slapButton.Initialize(dir + slapButtonTex, 1, .5f);
	slapButton.SetScale(vec2(.075f));
	slapButton.SetPosition(vec2(.8f, -.8f));
	InitShapes();
	printf("Emoji now %s", emojiTypeNames[current]);
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

