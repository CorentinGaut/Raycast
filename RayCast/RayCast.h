#pragma once
#include <vector>
#include "Vector.h"

using namespace std;

struct Sphere {
	double rayon;
	Vec3 <double> position;
	Vec3 <double> couleur;
	double albedo;
};

struct Ray {
	Vec3<double> position;
	Vec3<double> direction;
};

struct Lumiere {
	Vec3<double> position;
	Vec3<double> couleur;
	int intensite;
};


const int H = 600;
const int W = 600;
const int rebondMax = 20;
const int nbLumieresSurface = 50;
const double coeffreflexion = 0.3;

int indexAlbedo = 0.1;

Vec3<double> positionPerspective{ 300, 300, -1000 };

//----------------------couleur----------------------
Vec3<double> white{ 255, 255, 255 };
Vec3<double> green{ 0, 255, 0 };
Vec3<double> black{ 0, 0, 0 };
Vec3<double> grey{ 10, 10, 50 };
Vec3<double> red{ 255, 0, 0 };
Vec3<double> blue{ 0, 0, 255 };
Vec3<double> yellow{ 255, 255, 0 };

//----------------------Lumiere----------------------
Lumiere light{ Vec3<double>{ 590, 300, 100}, Vec3<double>{ 255, 255, 255}, 100 };
Lumiere posLumierSurfacique{ Vec3<double>{ 250,20,100 }, Vec3<double>{ 255, 255, 255}, 100 };
Lumiere posLumierSurfacique2{ Vec3<double>{ 550,250,100 }, Vec3<double>{ 0, 0, 125}, 400 };
Lumiere posLumierSurfacique3{ Vec3<double>{ 0,0,0 }, white, 600 };

//----------------------Objets----------------------
Sphere sphere{ 100, Vec3<double>{150, 300, 300}, white, 1 };
Sphere sphere1{ 20, Vec3<double>{400, 200, 400}, white, 0.2 };
Sphere sphere2{ 50, Vec3<double>{300, 500, 300}, blue, 0.1 };

Sphere spherefond{ 9500, Vec3<double>{300, 300, 10100}, red, 0.2 };
Sphere spheredevant{ 9500, Vec3<double>{300, 300, -10100}, white, 0.2 };
Sphere spheresol{ 9500, Vec3<double>{300, 10100, 500}, white, 0.95 };
Sphere sphereplafond{ 9500, Vec3<double>{300, -9550, 500}, green, 0.2 };
Sphere sphereDroit{ 9500, Vec3<double>{-9550, 300, 500}, blue, 0.2 };
Sphere spheregauche{ 9500, Vec3<double>{9550 + 600, 300, 500}, yellow, 0.2 };

//----------------------tableau----------------------
vector<Sphere> objetsScenes;
vector<Lumiere> LumieresScenes;
