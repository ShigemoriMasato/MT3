#pragma once
#include "MyMath.h"

bool IsCollition(const Sphere& sphere, const Plane& plane);

bool IsCollition(const Segment& segment, const Plane& plane);

bool IsCollition(const Segment& segment, const Triangle& triangle);

bool IsCollition(const AABB& a, const AABB& b);

bool IsCollition(const AABB& aabb, const Sphere& sphere);
