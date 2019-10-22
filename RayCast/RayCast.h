#pragma once
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <optional>
#include <algorithm>
#include "Nodes.h"
#include "Vector.h"

using namespace std;

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
const int rebondMax = 10;
const int nbLumieresSurface = 300;
const double coeffreflexion = 0.3;

double indexAlbedo = 0.1;

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
Lumiere light1{ Vec3<double>{ 50, 590, 0}, Vec3<double>{ 255, 255, 255}, 1000 };
Lumiere posLumierSurfacique{ Vec3<double>{ 250,20,100 }, Vec3<double>{ 255, 255, 255}, 1000000 };
Lumiere posLumierSurfacique2{ Vec3<double>{ 550,250,100 }, Vec3<double>{ 0, 0, 125}, 400000 };
Lumiere posLumierSurfacique3{ Vec3<double>{ 0,550,0 }, white, 60000 };

//----------------------Objets----------------------
Sphere sphere{ 100, Vec3<double>{150, 300, 300}, white, 0.1 };
Sphere sphere1{ 20, Vec3<double>{400, 200, 400}, white, 0.2 };
Sphere sphere2{ 50, Vec3<double>{350, 450, 100}, red, 0.1 };
Sphere sphere3{ 50, Vec3<double>{500, 400, 300}, red, 0.1 };
Sphere sphere4{ 50, Vec3<double>{75, 75, 300}, red, 0.1 };

Sphere spherefond{ 9500, Vec3<double>{300, 300, 10100}, red, 0.2 };
Sphere spheredevant{ 9500, Vec3<double>{300, 300, -10100}, white, 0.2 };
Sphere spheresol{ 9500, Vec3<double>{300, 10100, 500}, white, 0.1 };
Sphere sphereplafond{ 9500, Vec3<double>{300, -9550, 500}, green, 0.2 };
Sphere sphereDroit{ 9500, Vec3<double>{-9550, 300, 500}, blue, 0.2 };
Sphere spheregauche{ 9500, Vec3<double>{9550 + 600, 300, 500}, yellow, 0.2 };

//----------------------tableau----------------------
vector<Sphere> objetsScenes;
vector<Lumiere> LumieresScenes;

