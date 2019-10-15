#pragma once
#include "Vector.h"
#include <variant>

struct Sphere {
	double rayon;
	Vec3 <double> position;
	Vec3 <double> couleur;
	double albedo;
};

struct BoundingBox {
	Vec3<double> minPos;
	Vec3<double> maxPos;
};


struct Leaf {
	Sphere sphere;
};

class Nodes
{
public:
	BoundingBox bb;

	std::variant<Nodes*, Leaf> first;
	std::variant<Nodes*, Leaf> second;

	Nodes(std::variant<Nodes*, Leaf> f, std::variant<Nodes*, Leaf> s, BoundingBox b);

};

