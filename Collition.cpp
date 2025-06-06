#include "Collition.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

using namespace MyMath;

float ClosestPoint(const Segment& segment, const Vector3& normal, const float distance) {
	float bn = dot(segment.diff, normal);
	if (bn == 0.0f) {
		return 0.0f; // 平行な場合は距離を返す
	}
	float on = dot(segment.origin, normal);
	float t = (distance - on) / bn;
	return t;
}

bool IsCollition(const Sphere& sphere, const Plane& plane) {
	float distance = dot(sphere.center, plane.normal) - plane.distance;
	if (distance < sphere.radius && -sphere.radius < distance) {
		return true;
	}
	return false;
}

bool IsCollition(const Segment& segment, const Plane& plane) {
	float t = ClosestPoint(segment, plane.normal, plane.distance);
	if (t < 0.0f || t > 1.0f) {
		return false;
	}
	return true;
}

bool IsCollition(const Segment& segment, const Triangle& triangle) {
	Vector3 normal = cross(triangle.vertices[1] - triangle.vertices[0], triangle.vertices[2] - triangle.vertices[1]).Normalize();
	float t = ClosestPoint(segment, normal, dot(normal, triangle.vertices[0]));
	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	Vector3 hitpoint = segment.origin + segment.diff * t;

	Vector3 center = (triangle.vertices[0] + triangle.vertices[1] + triangle.vertices[2]) / 3.0f;
	Vector3 center01 = center - (triangle.vertices[0] + triangle.vertices[1]) / 2;
	Vector3 center12 = center - (triangle.vertices[1] + triangle.vertices[2]) / 2;
	Vector3 center20 = center - (triangle.vertices[2] + triangle.vertices[0]) / 2;
	Vector3 p01 = hitpoint - (triangle.vertices[0] + triangle.vertices[1]) / 2;
	Vector3 p12 = hitpoint - (triangle.vertices[1] + triangle.vertices[2]) / 2;
	Vector3 p20 = hitpoint - (triangle.vertices[2] + triangle.vertices[0]) / 2;

	float a = dot(center01.Normalize(), p01.Normalize());
	float b = dot(center12.Normalize(), p12.Normalize());
	float c = dot(center20.Normalize(), p20.Normalize());

	if (a >= 0.0f &&
		b >= 0.0f &&
		c >= 0.0f) {
		return true;
	}

	return false;
}

bool IsCollition(const AABB& a, const AABB& b) {
	return (a.min.x <= b.max.x && a.max.x >= b.min.x &&
		a.min.y <= b.max.y && a.max.y >= b.min.y &&
		a.min.z <= b.max.z && a.max.z >= b.min.z);
}

bool IsCollition(const AABB& aabb, const Sphere& sphere) {
	Vector3 closestPoint = clamp(sphere.center, aabb.min, aabb.max);
	float distance = (sphere.center - closestPoint).Length();
	return distance <= sphere.radius;
}

bool IsCollition(const AABB& aabb, const Segment& segment) {
	Vector3 tNear;
	Vector3 tFar;
	Vector3 normal[] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1 }
	};

	for (int i = 0; i < 3; ++i) {
		
		float tmin = ClosestPoint(segment, normal[i], (&aabb.min.x)[i]);
		float tmax = ClosestPoint(segment, normal[i], (&aabb.max.x)[i]);

		(&tNear.x)[i] = min(tmin, tmax);
		(&tFar.x)[i] = max(tmin, tmax);
	}

	float tmin = max(max(tNear.x, tNear.y), tNear.z);
	float tmax = min(min(tFar.x, tFar.y), tFar.z);

	ImGui::Begin("AABB");
	ImGui::Text("tNear: (%f, %f, %f)", tNear.x, tNear.y, tNear.z);
	ImGui::Text("tFar: (%f, %f, %f)", tFar.x, tFar.y, tFar.z);
	ImGui::Text("tmin: %f, tmax: %f", tmin, tmax);
	ImGui::Text("Collision: %s", (tmin <= tmax) ? "Yes" : "No");
	ImGui::End();

	if (tmin <= tmax) {
		return true;
	}

	return false;
}
