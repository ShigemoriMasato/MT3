#pragma once

struct Matrix4x4 {
	float m[4][4];
};

struct Vector2 {
	float x;
	float y;
	float z;
};

struct Vector3 {
	float x;
	float y;
	float z;

	float Length()const;

	Vector3 Normalize();
};

Vector3 operator+(const Vector3& a, const Vector3& b);
Vector3 operator-(const Vector3& a, const Vector3& b);
Vector3 operator*(const Vector3& a, const Vector3& b);
Vector3 operator*(const Vector3& a, float b);
Vector3 operator*(float a, const Vector3& b);
Vector3 operator/(const Vector3& a, float b);
Vector3 operator/(const Vector3& a, const Vector3& b);
Vector3 operator*(const Matrix4x4& a, const Vector3& b);
Vector3 operator*(const Vector3& a, const Matrix4x4& b);
Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);

Vector3 operator+=(Vector3& a, const Vector3& b);
Vector3 operator-=(Vector3& a, const Vector3& b);
Vector3 operator*=(Vector3& a, const Vector3& b);
Vector3 operator*=(Vector3& a, float b);
Vector3 operator*=(const float a, const Vector3& b);
Vector3 operator/=(Vector3& a, float b);
Vector3 operator/=(Vector3& a, const Vector3& b);
Vector3 operator/=(const float a, const Vector3& b);
bool operator==(const Vector3& a, const Vector3& b);
bool operator!=(const Vector3& a, const Vector3& b);


struct Sphere {
	Vector3 center;
	float radius;
	unsigned int subdivision;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Plane {
	Vector3 normal;
	float distance;
};

struct Triangle {
	Vector3 vertices[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateXMatrix(float radius);

Matrix4x4 MakeRotateYMatrix(float radius);

Matrix4x4 MakeRotateZMatrix(float radius);

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat, const char* label);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 Inverse(const Matrix4x4& a);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

namespace MyMath {

	float cot(float radian);

	Vector3 cross(const Vector3& a, const Vector3& b);

	float dot(const Vector3& a, const Vector3& b);

	unsigned int ColorLarp(unsigned int color, unsigned int targetColor, float rate);

	Vector3 Project(const Vector3& v1, const Vector3& v2);

	Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	Vector3 Perpendicular(const Vector3& vector);

	Vector3 clamp(const Vector3& value, const Vector3& min, const Vector3& max);

}
