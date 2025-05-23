#include "MyMath.h"
#include <cmath>
#include <cassert>
#include <Novice.h>

using namespace MyMath;

Vector3 operator+(const Vector3& a, const Vector3& b) {
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 operator*(const Vector3& a, const Vector3& b) {
	return { a.x * b.x, a.y * b.y, a.z * b.z };
}

Vector3 operator*(const Vector3& a, float b) {
	return { a.x * b, a.y * b, a.z * b };
}

Vector3 operator*(float a, const Vector3& b) {
	return { a * b.x, a * b.y, a * b.z };
}

Vector3 operator/(const Vector3& a, float b) {
	assert(b != 0.0f);
	return { a.x / b, a.y / b, a.z / b };
}

Vector3 operator/(const Vector3& a, const Vector3& b) {
	return { a.x / b.x, a.y / b.y, a.z / b.z };
}

Vector3 operator*(const Matrix4x4& a, const Vector3& b) {
	Vector3 ans;
	ans.x = b.x * a.m[0][0] + b.y * a.m[1][0] + b.z * a.m[2][0] + a.m[3][0];
	ans.y = b.x * a.m[0][1] + b.y * a.m[1][1] + b.z * a.m[2][1] + a.m[3][1];
	ans.z = b.x * a.m[0][2] + b.y * a.m[1][2] + b.z * a.m[2][2] + a.m[3][2];
	float w = b.x * a.m[0][3] + b.y * a.m[1][3] + b.z * a.m[2][3] + a.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;
}

Vector3 operator*(const Vector3& a, const Matrix4x4& b) {
	Vector3 ans{};
	ans.x = a.x * b.m[0][0] + a.y * b.m[1][0] + a.z * b.m[2][0] + b.m[3][0];
	ans.y = a.x * b.m[0][1] + a.y * b.m[1][1] + a.z * b.m[2][1] + b.m[3][1];
	ans.z = a.x * b.m[0][2] + a.y * b.m[1][2] + a.z * b.m[2][2] + b.m[3][2];
	float w = a.x * b.m[0][3] + a.y * b.m[1][3] + a.z * b.m[2][3] + b.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;
}

Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
}

Vector3 operator/(const float a, const Vector3& b) {
	return { a / b.x, a / b.y, a / b.z };
}

Vector3 operator+=(Vector3& a, const Vector3& b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vector3 operator-=(Vector3& a, const Vector3& b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

Vector3 operator*=(Vector3& a, const Vector3& b) {
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}

Vector3 operator*=(Vector3& a, float b) {
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

Vector3 operator*=(const float a, const Vector3& b) {
	return b * a;
}

Vector3 operator/=(Vector3& a, float b) {
	assert(b != 0.0f);
	a.x /= b;
	a.y /= b;
	a.z /= b;
	return a;
}

Vector3 operator/=(Vector3& a, const Vector3& b) {
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return a;
}

Vector3 operator/=(const float a, const Vector3& b) {
	return b * a;
}

bool operator==(const Vector3& a, const Vector3& b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

bool operator!=(const Vector3& a, const Vector3& b) {
	return !(a == b);
}

float MyMath::cot(float radian) {
	return 1.0f / tanf(radian);
}

Vector3 MyMath::cross(const Vector3& a, const Vector3& b) {
	return { b.y * a.z - b.z * a.y, b.z * a.x - b.x * a.z, b.x * a.y - b.y * a.x };
}

//debug用
unsigned int MyMath::ColorLarp(unsigned int color, unsigned int targetColor, float rate) {
	unsigned int r = (color >> 24) & 0xFF;
	unsigned int g = (color >> 16) & 0xFF;
	unsigned int b = (color >> 8) & 0xFF;
	unsigned int targetR = (targetColor >> 24) & 0xFF;
	unsigned int targetG = (targetColor >> 16) & 0xFF;
	unsigned int targetB = (targetColor >> 8) & 0xFF;
	r += int((targetR - r) * rate);
	g += int((targetG - g) * rate);
	b += int((targetB - b) * rate);
	return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

float MyMath::dot(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1
	};

}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	return {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};

}

Matrix4x4 MakeRotateXMatrix(float radius) {

	return {
		1, 0, 0, 0,
		0, std::cosf(radius), std::sinf(radius), 0,
		0, -std::sinf(radius), std::cosf(radius), 0,
		0, 0, 0, 1
	};

}

Matrix4x4 MakeRotateYMatrix(float radius) {

	return {
		std::cosf(radius), 0, -std::sinf(radius), 0,
		0, 1, 0, 0,
		std::sinf(radius), 0, std::cosf(radius), 0,
		0, 0, 0, 1
	};

}

Matrix4x4 MakeRotateZMatrix(float radius) {

	return { std::cosf(radius), std::sinf(radius), 0, 0,
		-std::sinf(radius), std::cosf(radius), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };

}

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	return Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 1; i < 5; ++i) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(x + j * 80, y + i * 20, "%.2f", mat.m[i - 1][j]);
		}
	}
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	return {
		cot(fovY / 2) / aspectRatio, 0, 0, 0,
		0, cot(fovY / 2), 0, 0,
		0, 0, farClip / (farClip - nearClip), 1,
		0, 0, (-nearClip * farClip) / (farClip - nearClip), 0
	};
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	return {
		2 / (right - left), 0, 0, 0,
		0, 2 / (top - bottom), 0, 0,
		0, 0, 1 / (farClip - nearClip), 0,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1
	};
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
		width / 2, 0, 0, 0,
		0, -height / 2, 0, 0,
		0, 0, maxDepth - minDepth, 0,
		left + width / 2, top + height / 2, minDepth, 1
	};
}

Matrix4x4 Inverse(const Matrix4x4& a) {

	float d = a.m[0][0] * a.m[1][1] * a.m[2][2] * a.m[3][3]
		+ a.m[0][0] * a.m[1][2] * a.m[2][3] * a.m[3][1]
		+ a.m[0][0] * a.m[1][3] * a.m[2][1] * a.m[3][2]
		- a.m[0][0] * a.m[1][3] * a.m[2][2] * a.m[3][1]
		- a.m[0][0] * a.m[1][2] * a.m[2][1] * a.m[3][3]
		- a.m[0][0] * a.m[1][1] * a.m[2][3] * a.m[3][2]
		- a.m[0][1] * a.m[1][0] * a.m[2][2] * a.m[3][3]
		- a.m[0][2] * a.m[1][0] * a.m[2][3] * a.m[3][1]
		- a.m[0][3] * a.m[1][0] * a.m[2][1] * a.m[3][2]
		+ a.m[0][3] * a.m[1][0] * a.m[2][2] * a.m[3][1]
		+ a.m[0][2] * a.m[1][0] * a.m[2][1] * a.m[3][3]
		+ a.m[0][1] * a.m[1][0] * a.m[2][3] * a.m[3][2]
		+ a.m[0][1] * a.m[1][2] * a.m[2][0] * a.m[3][3]
		+ a.m[0][2] * a.m[1][3] * a.m[2][0] * a.m[3][1]
		+ a.m[0][3] * a.m[1][1] * a.m[2][0] * a.m[3][2]
		- a.m[0][3] * a.m[1][2] * a.m[2][0] * a.m[3][1]
		- a.m[0][2] * a.m[1][1] * a.m[2][0] * a.m[3][3]
		- a.m[0][1] * a.m[1][3] * a.m[2][0] * a.m[3][2]
		- a.m[0][1] * a.m[1][2] * a.m[2][3] * a.m[3][0]
		- a.m[0][2] * a.m[1][3] * a.m[2][1] * a.m[3][0]
		- a.m[0][3] * a.m[1][1] * a.m[2][2] * a.m[3][0]
		+ a.m[0][3] * a.m[1][2] * a.m[2][1] * a.m[3][0]
		+ a.m[0][2] * a.m[1][1] * a.m[2][3] * a.m[3][0]
		+ a.m[0][1] * a.m[1][3] * a.m[2][2] * a.m[3][0];

	assert(fabsf(d) > 1e-6f);

	return {
		(a.m[1][1] * a.m[2][2] * a.m[3][3] + a.m[1][2] * a.m[2][3] * a.m[3][1] + a.m[1][3] * a.m[2][1] * a.m[3][2]
			- a.m[1][3] * a.m[2][2] * a.m[3][1] - a.m[1][2] * a.m[2][1] * a.m[3][3] - a.m[1][1] * a.m[2][3] * a.m[3][2]) / d,
		-(a.m[0][1] * a.m[2][2] * a.m[3][3] + a.m[0][2] * a.m[2][3] * a.m[3][1] + a.m[0][3] * a.m[2][1] * a.m[3][2]
			- a.m[0][3] * a.m[2][2] * a.m[3][1] - a.m[0][2] * a.m[2][1] * a.m[3][3] - a.m[0][1] * a.m[2][3] * a.m[3][2]) / d,
		(a.m[0][1] * a.m[1][2] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[3][1] + a.m[0][3] * a.m[1][1] * a.m[3][2]
			- a.m[0][3] * a.m[1][2] * a.m[3][1] - a.m[0][2] * a.m[1][1] * a.m[3][3] - a.m[0][1] * a.m[1][3] * a.m[3][2]) / d,
		-(a.m[0][1] * a.m[1][2] * a.m[2][3] + a.m[0][2] * a.m[1][3] * a.m[2][1] + a.m[0][3] * a.m[1][1] * a.m[2][2]
			- a.m[0][3] * a.m[1][2] * a.m[2][1] - a.m[0][2] * a.m[1][1] * a.m[2][3] - a.m[0][1] * a.m[1][3] * a.m[2][2]) / d,

		-(a.m[1][0] * a.m[2][2] * a.m[3][3] + a.m[1][2] * a.m[2][3] * a.m[3][0] + a.m[1][3] * a.m[2][0] * a.m[3][2]
			- a.m[1][3] * a.m[2][2] * a.m[3][0] - a.m[1][2] * a.m[2][0] * a.m[3][3] - a.m[1][0] * a.m[2][3] * a.m[3][2]) / d,
		(a.m[0][0] * a.m[2][2] * a.m[3][3] + a.m[0][2] * a.m[2][3] * a.m[3][0] + a.m[0][3] * a.m[2][0] * a.m[3][2]
			- a.m[0][3] * a.m[2][2] * a.m[3][0] - a.m[0][2] * a.m[2][0] * a.m[3][3] - a.m[0][0] * a.m[2][3] * a.m[3][2]) / d,
		-(a.m[0][0] * a.m[1][2] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[3][0] + a.m[0][3] * a.m[1][0] * a.m[3][2]
			- a.m[0][3] * a.m[1][2] * a.m[3][0] - a.m[0][2] * a.m[1][0] * a.m[3][3] - a.m[0][0] * a.m[1][3] * a.m[3][2]) / d,
		(a.m[0][0] * a.m[1][2] * a.m[2][3] + a.m[0][2] * a.m[1][3] * a.m[2][0] + a.m[0][3] * a.m[1][0] * a.m[2][2]
			- a.m[0][3] * a.m[1][2] * a.m[2][0] - a.m[0][2] * a.m[1][0] * a.m[2][3] - a.m[0][0] * a.m[1][3] * a.m[2][2]) / d,

		(a.m[1][0] * a.m[2][1] * a.m[3][3] + a.m[1][1] * a.m[2][3] * a.m[3][0] + a.m[1][3] * a.m[2][0] * a.m[3][1]
			- a.m[1][3] * a.m[2][1] * a.m[3][0] - a.m[1][1] * a.m[2][0] * a.m[3][3] - a.m[1][0] * a.m[2][3] * a.m[3][1]) / d,
		-(a.m[0][0] * a.m[2][1] * a.m[3][3] + a.m[0][1] * a.m[2][3] * a.m[3][0] + a.m[0][3] * a.m[2][0] * a.m[3][1]
			- a.m[0][3] * a.m[2][1] * a.m[3][0] - a.m[0][1] * a.m[2][0] * a.m[3][3] - a.m[0][0] * a.m[2][3] * a.m[3][1]) / d,
		(a.m[0][0] * a.m[1][1] * a.m[3][3] + a.m[0][1] * a.m[1][3] * a.m[3][0] + a.m[0][3] * a.m[1][0] * a.m[3][1]
			- a.m[0][3] * a.m[1][1] * a.m[3][0] - a.m[0][1] * a.m[1][0] * a.m[3][3] - a.m[0][0] * a.m[1][3] * a.m[3][1]) / d,
		-(a.m[0][0] * a.m[1][1] * a.m[2][3] + a.m[0][1] * a.m[1][3] * a.m[2][0] + a.m[0][3] * a.m[1][0] * a.m[2][1]
			- a.m[0][3] * a.m[1][1] * a.m[2][0] - a.m[0][1] * a.m[1][0] * a.m[2][3] - a.m[0][0] * a.m[1][3] * a.m[2][1]) / d,

		-(a.m[1][0] * a.m[2][1] * a.m[3][2] + a.m[1][1] * a.m[2][2] * a.m[3][0] + a.m[1][2] * a.m[2][0] * a.m[3][1]
			- a.m[1][2] * a.m[2][1] * a.m[3][0] - a.m[1][1] * a.m[2][0] * a.m[3][2] - a.m[1][0] * a.m[2][2] * a.m[3][1]) / d,
		(a.m[0][0] * a.m[2][1] * a.m[3][2] + a.m[0][1] * a.m[2][2] * a.m[3][0] + a.m[0][2] * a.m[2][0] * a.m[3][1]
			- a.m[0][2] * a.m[2][1] * a.m[3][0] - a.m[0][1] * a.m[2][0] * a.m[3][2] - a.m[0][0] * a.m[2][2] * a.m[3][1]) / d,
		-(a.m[0][0] * a.m[1][1] * a.m[3][2] + a.m[0][1] * a.m[1][2] * a.m[3][0] + a.m[0][2] * a.m[1][0] * a.m[3][1]
			- a.m[0][2] * a.m[1][1] * a.m[3][0] - a.m[0][1] * a.m[1][0] * a.m[3][2] - a.m[0][0] * a.m[1][2] * a.m[3][1]) / d,
		(a.m[0][0] * a.m[1][1] * a.m[2][2] + a.m[0][1] * a.m[1][2] * a.m[2][0] + a.m[0][2] * a.m[1][0] * a.m[2][1]
			- a.m[0][2] * a.m[1][1] * a.m[2][0] - a.m[0][1] * a.m[1][0] * a.m[2][2] - a.m[0][0] * a.m[1][2] * a.m[2][1]) / d
	};
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 ans;
	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;
}

float Vector3::Length() const {
	return std::sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize() {
	return { x / Length(), y / Length(), z / Length() };
}
