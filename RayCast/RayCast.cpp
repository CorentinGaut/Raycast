#include <fstream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <math.h>
#include <optional>
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
const int nbLumieresSurface = 50;

Vec3<double> positionPerspective{ 300, 300, -1000 };

//----------------------couleur----------------------
Vec3<double> white{ 255, 255, 255 };
Vec3<double> green{ 0, 255, 0 };
Vec3<double> black{ 0, 0, 0 };
Vec3<double> grey{ 10, 10, 50 };
Vec3<double> red{ 255, 0, 0 };
Vec3<double> blue{ 0, 0, 255 };
Vec3<double> yellow{ 255, 255, 0 };

Vec3<double> pix_col(black);

//----------------------Lumiere----------------------
Lumiere light{ Vec3<double>{ 590, 300, 100}, Vec3<double>{ 255, 255, 255}, 100 };
Lumiere posLumierSurfacique{ Vec3<double>{ 250,20,100 }, Vec3<double>{ 255, 255, 255}, 100 };

//----------------------Objets----------------------
Sphere sphere{ 100, Vec3<double>{150, 300, 300}, white, 1 };
Sphere sphere1{ 20, Vec3<double>{400, 200, 400}, white, 0 };
Sphere sphere2{ 30, Vec3<double>{350, 200, 300}, white, 0 };

Sphere spherefond{ 9500, Vec3<double>{300, 300, 10100}, red, 0 };
Sphere spheresol{ 9500, Vec3<double>{300, 10100, 500}, white, 0 };
Sphere sphereplafond{ 9500, Vec3<double>{300, -9550, 500}, green, 0 };
Sphere sphereDroit{ 9500, Vec3<double>{-9550, 300, 500}, blue, 0 };
Sphere spheregauche{ 9500, Vec3<double>{9550 + 600, 300, 500}, yellow, 0 };

//----------------------tableau----------------------
vector<Sphere> objetsScenes;
vector<Lumiere> LumieresScenes;

//fonction d'intersection entre une sphere et un rayon de lumiere
std::optional<double> intersect(Sphere S, Ray L) {
	double a = 1/*dot2(L.direction)*/;
	double b = 2 * (dot(L.position, L.direction) - dot(S.position, L.direction));
	double c = dot(L.position, L.position) + dot(S.position, S.position) - (2 * dot(S.position, L.position)) - (S.rayon * S.rayon);

	double delta = (b * b) - (4 * a * c);

	if (delta >= 0) {
		delta = sqrt(delta);
		const double t0 = (-b - delta)/2;
		const double t1 = (-b + delta)/2;
		if (t0 >= 0) {
			return t0;
		}
		else if (t1 >= 0)
		{
			return t1;
		}
	}
	else
	{
		return nullopt;
	}
}

optional<double> Calcul_t_min(vector<Sphere> spheres, Ray r) {
	std::optional<double> t_min = nullopt;
	//test si une sphere est derriere l'autre
	for (int i = 0; i < spheres.size(); i++) {
		std::optional<double> t = intersect(spheres[i], r);
		if ((!t_min.has_value()) || (t.has_value() && t.value() < t_min.value())) {
			t_min = t;
		}
	}
	return t_min;
}

Vec3<double> RayCast(Ray ray) {
	//calcul du t_min entre les spheres pour savoir s'il y en a une devant l'autre de la camera
	Vec3<double> result(black);
	int index = 0;
	std::optional<double> t_min = nullopt;
	for (int i = 0; i < objetsScenes.size(); i++) {
		std::optional<double> t = intersect(objetsScenes[i], ray);
		if ((!t_min.has_value() && t.has_value()) || (t.has_value() && t.value() < t_min.value())) {
			index = i;
			t_min = t;
		}
	}

	if (t_min.has_value()) {
		//sphere miroir
		if (objetsScenes[index].albedo >= 1) {
			Vec3<double> posIntersection = ray.position + (ray.direction * (t_min.value() - 0.1));
			Vec3<double> normalIntersection = posIntersection - objetsScenes[index].position;
			Vec3<double> rebondMiroir = normalIntersection * 2 * dot(ray.direction * (-1), normalize(normalIntersection)) + ray.direction;

			Ray rebond{ posIntersection,
						rebondMiroir };
			return RayCast(rebond);
		}

		for (int l = 0; l < LumieresScenes.size(); l++) {
			for (int h = 0; h < nbLumieresSurface; h++) {

				//generation d'une lumiere aleatoire pour les lumieres surfaciques
				int randX = rand() % 20;
				int randY = rand() % 19;
				int randZ = rand() % 19;
				Lumiere lightSu{ Vec3<double>{LumieresScenes[l].position.x + randX,
											  LumieresScenes[l].position.y + randY,
											  LumieresScenes[l].position.z + randZ}, white, 5 };

				Vec3<double> posIntersection = ray.position + (ray.direction * (t_min.value() - 0.1));
				Vec3<double> DirectionLumiere = lightSu.position - posIntersection;
				Vec3<double> normalIntersection = normalize(posIntersection - objetsScenes[index].position);
				double dt = abs(dot(normalize(DirectionLumiere), normalIntersection));

				//ajout de 0.1 pour pas que la sphere se teste elle-même
				Ray rebond{
					posIntersection + (normalize(DirectionLumiere)),
					normalize(DirectionLumiere)
				};

				//test s'il y a une sphere entre la sphere testée et la lumiere
				std::optional<double> rebondIntersecte_min = nullopt;
				for (int i = 0; i < objetsScenes.size(); i++) {
					std::optional<double> t = intersect(objetsScenes[i], rebond);
					if ((!rebondIntersecte_min.has_value()) || (t.has_value() && t.value() < rebondIntersecte_min.value())) {
						rebondIntersecte_min = t;
					}
				}

				if (!rebondIntersecte_min.has_value()) {

						result = result + objetsScenes[index].couleur * lightSu.couleur * dt / (norm(DirectionLumiere) * LumieresScenes.size() * nbLumieresSurface);
					
				}
				//test si une sphere a intersecter un objet derriere la lumiere 
				else if (rebondIntersecte_min.value() >= norm(DirectionLumiere))
				{

						result = result + objetsScenes[index].couleur * lightSu.couleur * dt / (norm(DirectionLumiere) * LumieresScenes.size() * nbLumieresSurface);
					
				}
				else
				{
					result = black + result;
				}
			}
		}
	}
	return result;
}


int main() {
	//en tête du fichier ppm
	std::ofstream out("out.ppm");
	out << "P3\n" << W << ' ' << H << ' ' << "255\n";

	//----------------------ajout des spheres----------------------
	objetsScenes.push_back(sphere);
	objetsScenes.push_back(sphere1);
	objetsScenes.push_back(spherefond);
	objetsScenes.push_back(spheresol);
	objetsScenes.push_back(sphereplafond);
	objetsScenes.push_back(sphereDroit);
	objetsScenes.push_back(spheregauche);

	//----------------------ajout des Lumieres----------------------
	//LumieresScenes.push_back(light);
	LumieresScenes.push_back(posLumierSurfacique);
	LumieresScenes.push_back(posLumierSurfacique);

	//----------------------Debut du dessin-------------------------

	for (int y = 0; y < H; ++y) {
		for (int x = 0; x < W; ++x) {

			pix_col = black;
			Vec3<double> directionCamera = normalize(Vec3<double>{ (double)x, (double)y, 0 } - positionPerspective);

			Ray ray{
				Vec3<double>{(double)x, (double)y, 0},
				directionCamera
			};

			Vec3<double> col = RayCast(ray);

			clamp255(col);
			out << (int)col.x << ' '
				<< (int)col.y << ' '
				<< (int)col.z << '\n';
		}
	}
}