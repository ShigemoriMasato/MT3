#include "Collition.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace MyMath;

bool IsCollition(const Sphere& sphere, const Plane& plane) {
	float distance = dot(sphere.center, plane.normal) - plane.distance;
	if (distance < sphere.radius && -sphere.radius < distance) {
		return true;
	}
	return false;
}

bool IsCollition(const Segment& segment, const Plane& plane) {
	float bn = dot(segment.diff, plane.normal);
	if (bn == 0.0f) {
		return false;
	}
	float on = dot(segment.origin, plane.normal);
	float t = (plane.distance - on) / bn;
	if (t < 0.0f || t > 1.0f) {
		return false;
	}
	return true;
}

bool IsCollition(const Segment& segment, const Triangle& triangle) {
	Plane plane;
	plane.normal = cross(triangle.vertices[1] - triangle.vertices[0], triangle.vertices[2] - triangle.vertices[1]).Normalize();
	plane.distance = dot(plane.normal, triangle.vertices[0]);

	float bn = dot(segment.diff, plane.normal);
	if (bn == 0.0f) {
		return false;
	}
	float on = dot(segment.origin, plane.normal);
	float t = (plane.distance - on) / bn;
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
	float tNearX = std::fmin(aabb.min.x, aabb.max.x);
	float tFarX = std::fmax(aabb.min.x, aabb.max.x);
	float tNearY = std::fmin(aabb.min.y, aabb.max.y);
	float tFarY = std::fmax(aabb.min.y, aabb.max.y);
	float tNearZ = std::fmin(aabb.min.z, aabb.max.z);
	float tFarZ = std::fmax(aabb.min.z, aabb.max.z);

	float tmin = std::fmax(std::fmax(tNearX, tNearY), tNearZ);
	float tmax = std::fmin(std::fmin(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		return true;
	}
}
