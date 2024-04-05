#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <windows.h>
#include <vector>
#include <memory.h>
#include <conio.h>
#include <queue>


#define SCREEN_WIDTH 150
#define SCREEN_HEIGHT 50
#define ARROW_KEYS -32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77


HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
enum {x_axis, y_axis, z_axis};

using namespace std;

short screen[SCREEN_HEIGHT][SCREEN_WIDTH];
enum {EMPTY = 0, FRONT, BACK };

const double fovy = M_PI / 3;
const double aspect = 1;
const double f = 50;
const double n = 10;
const double camera_sensitivity = M_PI / 60;	// 카메라 회전 감도
const double camera_move_speed = 2;				// 카메라 이동 속도
const int fog_range = 60;						// 어디서부터 회색으로 보일 것인지

class vec4;
class mat4;
class Object;

class vec4 {
public:
	double x, y, z, w;

	vec4() : x(0), y(0), z(0), w(1) {}
	vec4(double X, double Y, double Z, double W = 1) : x(X), y(Y), z(Z), w(W) {}

	vec4 operator+(const vec4& other) { return vec4(x + other.x, y + other.y, z + other.z, 1); }
	vec4 operator-(const vec4& other) { return vec4(x - other.x, y - other.y, z - other.z, 1); }
	vec4 operator*(const double& k) { return vec4(x * k, y * k, z * k, w * k); }
	double operator*(const vec4& other) { return x * other.x + y * other.y + z * other.z; }
	double distance(const vec4& other) { return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2)); }
	
};

class mat4 {
public:
	double elements[16];
	/*
		0	1	2	3
		4	5	6	7
		8	9	10	11
		12	13	14	15
	*/

	mat4() {// make Identity Matrix
		memset(this->elements, 0, sizeof(this->elements));
		this->elements[0] = this->elements[5] = this->elements[10] = this->elements[15] = 1;
	}
	mat4(const mat4& orig) {// copy matrix
		for (int i = 0; i < 16; i++) this->elements[i] = orig.elements[i];
	}
	
	inline mat4& operator+=(const mat4& rhs) {
		for (int i = 0; i < 16; i++) this->elements[i] += rhs.elements[i];
		return *this;
	}
	inline mat4& operator-=(const mat4& rhs) {
		for (int i = 0; i < 16; i++) this->elements[i] -= rhs.elements[i];
		return *this;
	}
	inline mat4& operator*=(const double& k) {
		for (int i = 0; i < 16; i++) this->elements[i] *= k;
		return *this;
	}

	inline mat4 operator+(const mat4& rhs) {
		mat4 newMat(*this);
		newMat += rhs;
		return newMat;
	}
	inline mat4 operator-(const mat4& rhs) {
		mat4 newMat(*this);
		newMat -= rhs;
		return newMat;
	}
	inline mat4 operator*(const mat4& rhs) {
		mat4 newMat(*this);
		newMat.elements[0] = this->elements[0] * rhs.elements[0] + this->elements[1] * rhs.elements[4] + this->elements[2] * rhs.elements[8] + this->elements[3] * rhs.elements[12];
		newMat.elements[1] = this->elements[0] * rhs.elements[1] + this->elements[1] * rhs.elements[5] + this->elements[2] * rhs.elements[9] + this->elements[3] * rhs.elements[13];
		newMat.elements[2] = this->elements[0] * rhs.elements[2] + this->elements[1] * rhs.elements[6] + this->elements[2] * rhs.elements[10] + this->elements[3] * rhs.elements[14];
		newMat.elements[3] = this->elements[0] * rhs.elements[3] + this->elements[1] * rhs.elements[7] + this->elements[2] * rhs.elements[11] + this->elements[3] * rhs.elements[15];

		newMat.elements[4] = this->elements[4] * rhs.elements[0] + this->elements[5] * rhs.elements[4] + this->elements[6] * rhs.elements[8] + this->elements[7] * rhs.elements[12];
		newMat.elements[5] = this->elements[4] * rhs.elements[1] + this->elements[5] * rhs.elements[5] + this->elements[6] * rhs.elements[9] + this->elements[7] * rhs.elements[13];
		newMat.elements[6] = this->elements[4] * rhs.elements[2] + this->elements[5] * rhs.elements[6] + this->elements[6] * rhs.elements[10] + this->elements[7] * rhs.elements[14];
		newMat.elements[7] = this->elements[4] * rhs.elements[3] + this->elements[5] * rhs.elements[7] + this->elements[6] * rhs.elements[11] + this->elements[7] * rhs.elements[15];

		newMat.elements[8] = this->elements[8] * rhs.elements[0] + this->elements[9] * rhs.elements[4] + this->elements[10] * rhs.elements[8] + this->elements[11] * rhs.elements[12];
		newMat.elements[9] = this->elements[8] * rhs.elements[1] + this->elements[9] * rhs.elements[5] + this->elements[10] * rhs.elements[9] + this->elements[11] * rhs.elements[13];
		newMat.elements[10] = this->elements[8] * rhs.elements[2] + this->elements[9] * rhs.elements[6] + this->elements[10] * rhs.elements[10] + this->elements[11] * rhs.elements[14];
		newMat.elements[11] = this->elements[8] * rhs.elements[3] + this->elements[9] * rhs.elements[7] + this->elements[10] * rhs.elements[11] + this->elements[11] * rhs.elements[15];

		newMat.elements[12] = this->elements[12] * rhs.elements[0] + this->elements[13] * rhs.elements[4] + this->elements[14] * rhs.elements[8] + this->elements[15] * rhs.elements[12];
		newMat.elements[13] = this->elements[12] * rhs.elements[1] + this->elements[13] * rhs.elements[5] + this->elements[14] * rhs.elements[9] + this->elements[15] * rhs.elements[13];
		newMat.elements[14] = this->elements[12] * rhs.elements[2] + this->elements[13] * rhs.elements[6] + this->elements[14] * rhs.elements[10] + this->elements[15] * rhs.elements[14];
		newMat.elements[15] = this->elements[12] * rhs.elements[3] + this->elements[13] * rhs.elements[7] + this->elements[14] * rhs.elements[11] + this->elements[15] * rhs.elements[15];

		return newMat;
	}
	inline vec4 operator*(const vec4& vec) {
		double _x = this->elements[0] * vec.x + this->elements[1] * vec.y + this->elements[2] * vec.z + this->elements[3] * vec.w;
		double _y = this->elements[4] * vec.x + this->elements[5] * vec.y + this->elements[6] * vec.z + this->elements[7] * vec.w;
		double _z = this->elements[8] * vec.x + this->elements[9] * vec.y + this->elements[10] * vec.z + this->elements[11] * vec.w;
		double _w = this->elements[12] * vec.x + this->elements[13] * vec.y + this->elements[14] * vec.z + this->elements[15] * vec.w;
		return vec4(_x, _y, _z, _w);
	}

	inline mat4& transpose() {
		mat4 newMat;
		newMat.elements[0] = elements[0]; newMat.elements[1] = elements[4];
		newMat.elements[2] = elements[8]; newMat.elements[3] = elements[12];
		newMat.elements[4] = elements[1]; newMat.elements[5] = elements[5];
		newMat.elements[6] = elements[9]; newMat.elements[7] = elements[13];
		newMat.elements[8] = elements[2]; newMat.elements[9] = elements[6];
		newMat.elements[10] = elements[10]; newMat.elements[11] = elements[14];
		newMat.elements[12] = elements[3]; newMat.elements[13] = elements[7];
		newMat.elements[14] = elements[11]; newMat.elements[15] = elements[15];
		return newMat;
	}
	inline mat4& normarlize() {
		for (int col = 0; col < 4; col++) {
			double k = sqrt(pow(elements[col], 2) + pow(elements[col + 4], 2) + pow(elements[col + 8], 2) + pow(elements[col + 12], 2));
			elements[col] /= k;
			elements[col + 4] /= k;
			elements[col + 8] /= k;
			elements[col + 12] /= k;
		}
		return *this;
	}

	void print() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				printf("%.8lf  ", elements[i * 4 + j]);
			}
			cout << endl;
		}
		cout << endl;
	}

};

class line {
public:
	vec4 p1, p2;

	line(vec4 P1, vec4 P2) : p1(P1), p2(P2) {}

	void drawOnScreen(mat4& transform, mat4& vpt) {
		vec4 _p1 = transform * p1;
		vec4 _p2 = transform * p2;
		if (_p1.z > _p2.z) {
			vec4 temp = _p1;
			_p1 = _p2;
			_p2 = temp;
		}
		/* _p1 is always closer to camera */
		if (_p2.z < 0) return;
		if (_p1.z < 0) {
			vec4 dir = _p2 - _p1;
			_p1 = _p1 + (dir * (-_p1.z / dir.z));
		}
		double front_ratio = (fog_range - _p1.z) / (_p2.z - _p1.z);

		_p1 = _p1 * (1 / _p1.w);
		_p2 = _p2 * (1 / _p2.w);
		
		/* _p1, _p2 are in NDC */

		_p1 = vpt * _p1;
		_p2 = vpt * _p2;
		vec4 dir = _p2 - _p1;

		double distance = _p1.distance(_p2);
		double step = 1 / distance;
		double front_until_here = distance * front_ratio;

		if (front_until_here < 0) {
			for (int i = 0; i < distance; i++) {
				_p1 = _p1 + (dir * step);
				int r = (int)round(_p1.y);
				int c = (int)round(_p1.x);

				if (r < 0 || r >= SCREEN_HEIGHT || c < 0 || c >= SCREEN_WIDTH) continue;
				screen[r][c] |= BACK;
			}
		}
		else {
			for (int i = 0; i < distance; i++) {
				_p1 = _p1 + (dir * step);
				int r = (int)round(_p1.y);
				int c = (int)round(_p1.x);

				if (r < 0 || r >= SCREEN_HEIGHT || c < 0 || c >= SCREEN_WIDTH) continue;
				screen[r][c] |= (i < front_until_here ? FRONT : BACK);
			}
		}
		
		return;
	}
};

mat4 createAffineWSRotationMat4(const short axis, double angle) {
	mat4 newMat4;
	double s = sin(angle);
	double c = cos(angle);
	switch(axis) {
	case x_axis:
		newMat4.elements[5] = newMat4.elements[10] = c;
		newMat4.elements[9] = s; newMat4.elements[6] = -s;
		break;
	case y_axis:
		newMat4.elements[0] = newMat4.elements[10] = c;
		newMat4.elements[2] = s; newMat4.elements[8] = -s;
		break;
	case z_axis:
		newMat4.elements[0] = newMat4.elements[5] = c;
		newMat4.elements[4] = s; newMat4.elements[1] = -s;
		break;
	default:
		break;
	}
	return newMat4.normarlize();
}
mat4 createAffineTranslationMat4(const vec4& vec) {
	mat4 newMat4;
	newMat4.elements[3] = vec.x;
	newMat4.elements[7] = vec.y;
	newMat4.elements[11] = vec.z;
	return newMat4;
}
mat4 createAffineScalingMat4(const vec4& vec) {
	mat4 newMat;
	newMat.elements[0] = vec.x;
	newMat.elements[5] = vec.y;
	newMat.elements[10] = vec.z;
	return newMat;
}
mat4 createViewTransformMat4(Object camera);
mat4 createProjectionTransformMat4();
mat4 createViewportTransformMat4();


class Object {
public:
	vector<line> lines;
	mat4 pose;

	Object() {}
	void rotateWS(const short axis, double angle) {
		mat4 R = createAffineWSRotationMat4(axis, angle);
		pose = R * pose;
		return;
	}
	void rotateOS(const short axis, double angle) {
		vec4 t(pose.elements[3], pose.elements[7], pose.elements[11]);
		mat4 R(pose);
		R.elements[3] = R.elements[7] = R.elements[11] = 0;

		pose = mat4();
		this->rotateWS(axis, angle);
		pose = R * pose;
		pose.normarlize();
		this->translateWS(t);
		return;
	}
	void translateWS(const vec4& vec) {
		mat4 t = createAffineTranslationMat4(vec);
		pose = t * pose;
		return;
	}
	void translateOS(const vec4& vec) {
		vec4 t(pose.elements[3], pose.elements[7], pose.elements[11]);
		mat4 R(pose);
		R.elements[3] = R.elements[7] = R.elements[11] = 0;

		pose = mat4();
		this->translateWS(vec);
		pose = R * pose;
		this->translateWS(t);

	}
	void scale(const vec4& vec) {
		mat4 S = createAffineScalingMat4(vec);
		pose = S * pose;
	}
	void drawOnScreen(const Object camera) {
		mat4 vt = createViewTransformMat4(camera);
		mat4 pt = createProjectionTransformMat4();
		mat4 vpt = createViewportTransformMat4();

		mat4 transform = pt * (vt * pose);

		for (line _line : this->lines) {
			_line.drawOnScreen(transform, vpt);
		}

		return;
	}
};

mat4 createViewTransformMat4(Object camera) {
	vec4 EYE(camera.pose.elements[3], camera.pose.elements[7], camera.pose.elements[11]);
	mat4 R(camera.pose);
	R.elements[3] = R.elements[7] = R.elements[11] = 0;
	mat4 R_inv = R.transpose();

	mat4 T_inv = createAffineTranslationMat4(EYE * -1);

	return R_inv * T_inv;
}
mat4 createProjectionTransformMat4() {
	mat4 newMat;
	newMat.elements[0] = 1 / (aspect * tan(fovy / 2));
	newMat.elements[5] = 1 / tan(fovy / 2);
	newMat.elements[10] = -(f + n) / (f - n);
	newMat.elements[11] = -(2 * n * f) / (f - n);
	newMat.elements[14] = -1;
	newMat.elements[15] = 0;
	return newMat;
}
mat4 createViewportTransformMat4() {
	mat4 newMat = createAffineScalingMat4(vec4(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.5));
	newMat.elements[3] = SCREEN_WIDTH / 2;
	newMat.elements[7] = SCREEN_HEIGHT / 2;
	newMat.elements[11] = 0.5;
	return newMat;
}

void clearScreen() {
	memset(screen, 0, sizeof(screen));
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(std_output_handle, pos);
}

void drawScreen() {
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			if (screen[i][j] == 0) {
				printf(" ");
			}
			else if (screen[i][j] & 1) {
				SetConsoleTextAttribute(std_output_handle, 15);
				printf("@");
			}
			else {
				SetConsoleTextAttribute(std_output_handle, 8);
				printf("#");
			}
		}
		printf("\n");
	}
}

Object obj_cube(const int cube_size, int x, int y, int z) {
	Object cube;
	const int half = (int)cube_size / 2;

	cube.lines.push_back(line(vec4(half, half, half), vec4(-half, half, half)));
	cube.lines.push_back(line(vec4(half, half, half), vec4(half, -half, half)));
	cube.lines.push_back(line(vec4(half, half, half), vec4(half, half, -half)));

	cube.lines.push_back(line(vec4(half, -half, -half), vec4(-half, -half, -half)));
	cube.lines.push_back(line(vec4(half, -half, -half), vec4(half, half, -half)));
	cube.lines.push_back(line(vec4(half, -half, -half), vec4(half, -half, half)));

	cube.lines.push_back(line(vec4(-half, -half, half), vec4(half, -half, half)));
	cube.lines.push_back(line(vec4(-half, -half, half), vec4(-half, half, half)));
	cube.lines.push_back(line(vec4(-half, -half, half), vec4(-half, -half, -half)));

	cube.lines.push_back(line(vec4(-half, half, -half), vec4(half, half, -half)));
	cube.lines.push_back(line(vec4(-half, half, -half), vec4(-half, -half, -half)));
	cube.lines.push_back(line(vec4(-half, half, -half), vec4(-half, half, half)));

	cube.translateWS(vec4(x, y, z));
	return cube;
}
Object obj_straight_line(int x1, int y1, int z1, int x2, int y2, int z2) {
	Object stick;
	stick.lines.push_back(line(vec4(x1, y1, z1), vec4(x2, y2, z2)));
	return stick;
}

int main() {

	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(std_output_handle, &cursorInfo);
	char kb_input;

	/* Create Object */

	Object camera;
	camera.translateWS(vec4(0, 0, 40));
	Object cube1 = obj_cube(29, 0, 0, 0);

	/* End */

	string buffer;
	while (1) {
		/* Draw */

		clearScreen();
		cube1.drawOnScreen(camera);
		drawScreen();

		/* Auto actions per frame */
		
		cube1.rotateOS(y_axis, M_PI / 60);
		cube1.rotateOS(x_axis, M_PI / 60);


		/* Control */
		if (_kbhit()) {
			kb_input = _getch();
			if (kb_input == ARROW_KEYS) {
				kb_input = _getch();
				switch (kb_input) {
				case LEFT:
					camera.rotateOS(y_axis, camera_sensitivity);
					break;
				case RIGHT:
					camera.rotateOS(y_axis, -camera_sensitivity);
					break;
				case UP:
					camera.rotateOS(x_axis, -camera_sensitivity);
					break;
				case DOWN:
					camera.rotateOS(x_axis, camera_sensitivity);
					break;
				}
			}
			else if (kb_input == 97) camera.translateOS(vec4(-camera_move_speed, 0, 0));	// LEFT
			else if (kb_input == 100) camera.translateOS(vec4(camera_move_speed, 0, 0));	// RIGHT
			else if (kb_input == 119) camera.translateOS(vec4(0, 0, -camera_move_speed));	// FORWARD
			else if (kb_input == 115) camera.translateOS(vec4(0, 0, camera_move_speed));	// BACKWARD
		}
	}

}