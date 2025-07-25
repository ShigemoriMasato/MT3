#include "Draw.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace MyMath;

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGrigHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGrigHalfWidth * 2.0f) / kSubdivision;
	//奥から手前
	for (uint32_t i = 0; i <= kSubdivision; ++i) {
		Vector3 start = { -kGrigHalfWidth, 0.0f, -kGrigHalfWidth + kGridEvery * i };
		Vector3 end = { kGrigHalfWidth, 0.0f, -kGrigHalfWidth + kGridEvery * i };
		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xaaaaaaff);
	}

	//左から右
	for (uint32_t i = 0; i <= kSubdivision; ++i) {
		Vector3 start = { -kGrigHalfWidth + kGridEvery * i, 0.0f, -kGrigHalfWidth };
		Vector3 end = { -kGrigHalfWidth + kGridEvery * i, 0.0f, kGrigHalfWidth };
		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xaaaaaaff);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = sphere.subdivision;
	const float kLonEvery = 2.0f * (float)M_PI / kSubdivision;
	const float kLatEvery = (float)M_PI / kSubdivision;
	//緯度方向に分解
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = (float)-M_PI / 2 + kLatEvery * latIndex;
		float sinlat = std::sinf(lat);
		float coslat = std::cosf(lat);
		//経度方向に分解
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			float sinlon = std::sinf(lon);
			float coslon = std::cosf(lon);
			Vector3 a, b, c;
			a = { sphere.center.x + sphere.radius * coslat * coslon,
				sphere.center.y + sphere.radius * sinlat,
				sphere.center.z + sphere.radius * coslat * sinlon };
			b = { sphere.center.x + sphere.radius * std::cosf(lat + kLatEvery) * coslon,
				sphere.center.y + sphere.radius * std::sinf(lat + kLatEvery),
				sphere.center.z + sphere.radius * std::cosf(lat + kLatEvery) * sinlon };
			c = { sphere.center.x + sphere.radius * coslat * std::cosf(lon + kLonEvery),
				sphere.center.y + sphere.radius * sinlat,
				sphere.center.z + sphere.radius * coslat * std::sinf(lon + kLonEvery) };

			a = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			b = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			c = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(b.x), int(b.y), int(c.x), int(c.y), color);

			//if (a * cross(b, c) <= 0) {
			//	Novice::DrawTriangle(int(a.x), int(a.y), int(b.x), int(b.y),
			//		int(c.x), int(c.y), color, kFillModeSolid);
			//}
			
		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = plane.normal * plane.distance;
	Vector3 perpendiculars[4];
	perpendiculars[0] = Perpendicular(plane.normal).Normalize();
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t i = 0; i < 4; ++i) {
		Vector3 extend = perpendiculars[i] * 2.0f;
		Vector3 point = center + extend;
		points[i] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	//Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	//↓これの0を3に書き換えると四角形になる
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = segment.origin * viewProjectionMatrix * viewportMatrix;
	Vector3 end = (segment.origin + segment.diff) * viewProjectionMatrix * viewportMatrix;
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	for (int i = 0; i < 3; ++i) {
		Vector3 start = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(triangle.vertices[(i + 1) % 3], viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
	}
}

void DrawAABB(const AABB& a, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 vertex[8];
	for (int i = 0; i < 8; ++i) {
		Vector3 buffer;
		buffer.x = i % 2 == 0 ? a.min.x : a.max.x;
		buffer.y = i % 4 < 2 ? a.max.y : a.min.y;
		buffer.z = i < 4 ? a.min.z : a.max.z;
		vertex[i] = buffer * viewProjectionMatrix * viewportMatrix;
	}

	for (int i = 0; i < 8; i += 2) {
		//x軸
		Novice::DrawLine(static_cast<int>(vertex[i].x), static_cast<int>(vertex[i].y), static_cast<int>(vertex[i + 1].x), static_cast<int>(vertex[i + 1].y), color);

		//y軸
		if (i % 4 == 0) {
			Novice::DrawLine(static_cast<int>(vertex[i].x), static_cast<int>(vertex[i].y), static_cast<int>(vertex[(i + 2) % 8].x), static_cast<int>(vertex[(i + 2) % 8].y), color);
		} else {
			Novice::DrawLine(static_cast<int>(vertex[i - 1].x), static_cast<int>(vertex[i - 1].y), static_cast<int>(vertex[(i + 1) % 8].x), static_cast<int>(vertex[(i + 1) % 8].y), color);
		}

		//z軸
		Novice::DrawLine(static_cast<int>(vertex[i / 2].x), static_cast<int>(vertex[i / 2].y), static_cast<int>(vertex[(i / 2 + 4) % 8].x), static_cast<int>(vertex[(i / 2 + 4) % 8].y), color);
	}
}

void DrawBezierCurve(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Matrix4x4 wvpMatrix, int segments, unsigned int color) {
	for (int i = 0; i < segments - 1; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(segments);
		float next = static_cast<float>(i + 1) / static_cast<float>(segments);

		Vector3 start = lerp(p0, p1, t);
		Vector3 end = lerp(p1, p2, t);
		Vector3 bezierPoint = lerp(start, end, t);
		bezierPoint = bezierPoint * wvpMatrix;

		Vector3 nextStart = lerp(p0, p1, next);
		Vector3 nextEnd = lerp(p1, p2, next);
		Vector3 nextBezierPoint = lerp(nextStart, nextEnd, next);
		nextBezierPoint = nextBezierPoint * wvpMatrix;

		Novice::DrawLine(int(bezierPoint.x), int(bezierPoint.y), int(nextBezierPoint.x), int(nextBezierPoint.y), color);
	}
}

void DrawSpring(const Spring& spring, const Vector3& target, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	//始点=原点
	Vector3 start = spring.anchor * viewProjectionMatrix * viewportMatrix;
	Vector3 end = target * viewProjectionMatrix * viewportMatrix;
	
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

void DrawBall(const Ball* ball, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	//DrawSphereを流用する
	Sphere sphere;
	sphere.center = ball->position;
	sphere.radius = ball->radius;
	sphere.subdivision = 16;

	DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, color);
}

void DrawPendulum(const Pendulum& pendulum, const Vector3& target, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = pendulum.anchor * viewProjectionMatrix * viewportMatrix;
	Vector3 end = target * viewProjectionMatrix * viewportMatrix;

	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

void DrawConicalPendulum(const ConicalPendulum& conicalPendulum, const Vector3& target, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Matrix4x4 renderingpipeline = viewProjectionMatrix * viewportMatrix;

	Vector3 start = conicalPendulum.anchor * renderingpipeline;
	Vector3 end = target * renderingpipeline;

	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}
