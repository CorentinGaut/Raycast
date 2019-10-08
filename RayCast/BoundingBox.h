#pragma once
#include "Vector.h"
#include "RayCast.h"
#include <optional>
#include <algorithm>

struct Box {
	Vec3<double> minPos, maxPos;
};


std::optional<double> intersect(Box b, Ray r) {
	Vec3<double> dir = 1.0 / r.direction;

	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	double t1 = (b.minPos.x - r.position.x) * dir.x;
	double t2 = (b.maxPos.x - r.position.x) * dir.x;
	double t3 = (b.minPos.y - r.position.y) * dir.y;
	double t4 = (b.maxPos.y - r.position.y) * dir.y;
	double t5 = (b.minPos.z - r.position.z) * dir.z;
	double t6 = (b.maxPos.z - r.position.z) * dir.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		return nullopt;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		return nullopt;
	}
	return tmin;
}