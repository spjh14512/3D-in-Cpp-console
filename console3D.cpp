#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <windows.h>
#include <vector>
#include <memory.h>


#define SCREEN_WIDTH 150
#define SCREEN_HEIGHT 50
enum {x_axis, y_axis, z_axis};

using namespace std;

bool screen[SCREEN_HEIGHT][SCREEN_WIDTH];

class vec4 {
public:
	double x, y, z, w;

	vec4() : x(0), y(0), z(0), w(1) {}
	vec4(double X, double Y, double Z, double W = 1) : x(X), y(Y), z(Z), w(W) {}

	vec4 operator+(const vec4& other) { return vec4(x + other.x, y + other.y, z + other.z, 1); }
	vec4 operator-(const vec4& other) { return vec4(x - other.x, y - other.y, z - other.z, 1); }
	vec4 operator*(const double& k) { return vec4(x * k, y * k, z * k, w * k); }
	double operator*(const vec4& other) { return x * other.x + y * other.y + z * other.z; }
	
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
	inline mat4& operator*=(const mat4& rhs) {
		this->elements[0] = this->elements[0] * rhs.elements[0] + this->elements[1] * rhs.elements[4] + this->elements[2] * rhs.elements[8] + this->elements[3] * rhs.elements[12];
		this->elements[1] = this->elements[0] * rhs.elements[1] + this->elements[1] * rhs.elements[5] + this->elements[2] * rhs.elements[9] + this->elements[3] * rhs.elements[13];
		this->elements[2] = this->elements[0] * rhs.elements[2] + this->elements[1] * rhs.elements[6] + this->elements[2] * rhs.elements[10] + this->elements[3] * rhs.elements[14];
		this->elements[3] = this->elements[0] * rhs.elements[3] + this->elements[1] * rhs.elements[7] + this->elements[2] * rhs.elements[11] + this->elements[3] * rhs.elements[15];

		this->elements[4] = this->elements[4] * rhs.elements[0] + this->elements[5] * rhs.elements[4] + this->elements[6] * rhs.elements[8] + this->elements[7] * rhs.elements[12];
		this->elements[5] = this->elements[4] * rhs.elements[1] + this->elements[5] * rhs.elements[5] + this->elements[6] * rhs.elements[9] + this->elements[7] * rhs.elements[13];
		this->elements[6] = this->elements[4] * rhs.elements[2] + this->elements[5] * rhs.elements[6] + this->elements[6] * rhs.elements[10] + this->elements[7] * rhs.elements[14];
		this->elements[7] = this->elements[4] * rhs.elements[3] + this->elements[5] * rhs.elements[7] + this->elements[6] * rhs.elements[11] + this->elements[7] * rhs.elements[15];

		this->elements[8] = this->elements[8] * rhs.elements[0] + this->elements[9] * rhs.elements[4] + this->elements[10] * rhs.elements[8] + this->elements[11] * rhs.elements[12];
		this->elements[9] = this->elements[8] * rhs.elements[1] + this->elements[9] * rhs.elements[5] + this->elements[10] * rhs.elements[9] + this->elements[11] * rhs.elements[13];
		this->elements[10] = this->elements[8] * rhs.elements[2] + this->elements[9] * rhs.elements[6] + this->elements[10] * rhs.elements[10] + this->elements[11] * rhs.elements[14];
		this->elements[11] = this->elements[8] * rhs.elements[3] + this->elements[9] * rhs.elements[7] + this->elements[10] * rhs.elements[11] + this->elements[11] * rhs.elements[15];

		this->elements[12] = this->elements[12] * rhs.elements[0] + this->elements[13] * rhs.elements[4] + this->elements[14] * rhs.elements[8] + this->elements[15] * rhs.elements[12];
		this->elements[13] = this->elements[12] * rhs.elements[1] + this->elements[13] * rhs.elements[5] + this->elements[14] * rhs.elements[9] + this->elements[15] * rhs.elements[13];
		this->elements[14] = this->elements[12] * rhs.elements[2] + this->elements[13] * rhs.elements[6] + this->elements[14] * rhs.elements[10] + this->elements[15] * rhs.elements[14];
		this->elements[15] = this->elements[12] * rhs.elements[3] + this->elements[13] * rhs.elements[7] + this->elements[14] * rhs.elements[11] + this->elements[15] * rhs.elements[15];
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
		newMat *= rhs;
		return newMat;
	}
	inline vec4 operator*(const vec4& vec) {
		double _x = this->elements[0] * vec.x + this->elements[1] * vec.y + this->elements[2] * vec.z + this->elements[3] * vec.w;
		double _y = this->elements[4] * vec.x + this->elements[5] * vec.y + this->elements[6] * vec.z + this->elements[7] * vec.w;
		double _z = this->elements[8] * vec.x + this->elements[9] * vec.y + this->elements[10] * vec.z + this->elements[11] * vec.w;
		double _w = this->elements[12] * vec.x + this->elements[13] * vec.y + this->elements[14] * vec.z + this->elements[15] * vec.w;
		return vec4(_x, _y, _z, _w);
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

};

inline mat4 transpose(const mat4& mat) {
	mat4 newMat;
	newMat.elements[0] = mat.elements[0]; newMat.elements[1] = mat.elements[4];
	newMat.elements[2] = mat.elements[8]; newMat.elements[3] = mat.elements[12];
	newMat.elements[4] = mat.elements[1]; newMat.elements[5] = mat.elements[5];
	newMat.elements[6] = mat.elements[9]; newMat.elements[7] = mat.elements[13];
	newMat.elements[8] = mat.elements[2]; newMat.elements[9] = mat.elements[6];
	newMat.elements[10] = mat.elements[10]; newMat.elements[11] = mat.elements[14];
	newMat.elements[12] = mat.elements[3]; newMat.elements[13] = mat.elements[7];
	newMat.elements[14] = mat.elements[11]; newMat.elements[15] = mat.elements[15];
	return newMat;
}
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


class Object {
public:
	vector<vec4> vertices;
	mat4 pose;

	Object() {}
	void rotateWS(const short axis, double angle) {
		mat4 R = createAffineWSRotationMat4(axis, angle);
		pose = R * pose;
		return;
	}
	void rotateOS(const short axis, double angle) {
		vec4 t(pose.elements[3], pose.elements[7], pose.elements[11]);
		mat4 R_inv(pose);
		R_inv.elements[3] = R_inv.elements[7] = R_inv.elements[11] = 0;
		mat4 R = transpose(R_inv);

		this->translate(t * -1);
		pose = R * pose;
		this->rotateWS(axis, angle);
		pose = R_inv * pose;
		this->translate(t);
		return;
	}
	void translate(const vec4& vec) {
		mat4 t = createAffineTranslationMat4(vec);
		pose = t * pose;
		return;
	}
	void drawOnScreen() {
		for (const vec4& vertex : vertices) {
			vec4 _vertex = pose * vertex;
			screen[(int)round(SCREEN_HEIGHT - _vertex.y)][(int)round(_vertex.x)] = true;
		}
	}
};

void clearScreen() {
	memset(screen, false, sizeof(screen));
	system("cls");
}

void drawScreen() {
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			screen[i][j] ? printf("@") : printf("_");
		}
		printf("\n");
	}
}

int main() {
	/*Object cube;
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(i, -5, -5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(i, 5, -5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(i, 5, 5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(i, -5, 5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(-5, i, -5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(5, i, -5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(5, i, 5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(-5, i, 5));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(-5, -5, i));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(5, -5, i));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(5, 5, i));
	for (int i = -5; i < 6; i++) cube.vertices.push_back(vec4(-5, 5, i));

	cube.translate(vec4(75, 25, 20));

	while (1) {
		clearScreen();
		cube.drawOnScreen();
		drawScreen();
		cube.rotateOS(y_axis, M_PI / 60);
		cube.rotateOS(x_axis, M_PI / 60);
		Sleep(500);
	}*/

	Object stick;
	for (int i = -7; i < 8; i++) stick.vertices.push_back(vec4(0, i, 0));
	stick.translate(vec4(75, 25, 0));
	while (1) {
		clearScreen();
		stick.drawOnScreen();
		drawScreen();
		stick.translate(vec4(1, 0, 0));
		Sleep(100);
	}
}