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



int main() {

	const int H = 600;
	const int W = 600;
	const int nbLumieresSurface = 50;

	Vec3<double> white{ 255, 255, 255 };
	Vec3<double> black{ 0, 0, 0 };
	Vec3<double> grey{ 10, 10, 50 };
	Vec3<double> red{ 255, 0, 0 };

	Sphere sphere{ 100, Vec3<double>{150, 300, 300}, white };
	Sphere sphere1{ 20, Vec3<double>{400, 300, 200}, white };
	Sphere sphere2{ 30, Vec3<double>{350, 200, 100}, white };

	Sphere spherefond{ 9500, Vec3<double>{0, 0, 10000}, red };
	//Sphere spheresol{ 9500, Vec3<double>{-10000, 0, 0}, red };

	Lumiere light{ Vec3<double>{ 590, 300, 200}, Vec3<double>{ 255, 255, 255}, 100 };
	Lumiere light1{ Vec3<double>{ 590, 10, 100}, Vec3<double>{ 255, 255, }, 400 };


	//ne tête du fichier ppm
	std::ofstream out("out.ppm");
	out << "P3\n" << W << ' ' << H << ' ' << "255\n";

	Vec3<double> pix_col(black);

	vector<Sphere> objetsScenes;
	objetsScenes.push_back(sphere);
	objetsScenes.push_back(sphere1);
	//objetsScenes.push_back(sphere2);
	objetsScenes.push_back(spherefond);
	//objetsScenes.push_back(spheresol);

	vector<Lumiere> LumieresScenes;
	LumieresScenes.push_back(light);
	//LumieresScenes.push_back(light1);
;


	//lumiere surfacique
	Vec3<double> posLumierSurfacique { 250,20,150 };
	for (int i = 0; i < nbLumieresSurface; i++)
	{
		int randX = rand() % 20;
		int randY = rand() % 19;
		int randZ = rand() % 19;
		Lumiere lightSu{ Vec3<double>{posLumierSurfacique.x + randX,posLumierSurfacique.y + randY,posLumierSurfacique.z + randZ}, white, 5 };
		LumieresScenes.push_back(lightSu);
	}

	

	for (int y = 0; y < H; ++y) {
		for (int x = 0; x < W; ++x) {
			pix_col = black;
			Ray ray{ 
				Vec3<double>{(double)x, (double)y, 0}, 
				Vec3<double>{0, 0, 1} 
			};

			
			//calcul du t_min entre les spheres pour savoir s'il y en a une devant l'autre de la camera
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
				for (int l = 0; l < LumieresScenes.size(); l++) {
					Vec3<double> posIntersection = ray.position + (ray.direction * (t_min.value() + 0.1));
					Vec3<double> DirectionLumiere = LumieresScenes[l].position - posIntersection;
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
						pix_col = pix_col + ((LumieresScenes[l].couleur) * dt / norm(DirectionLumiere) * LumieresScenes[l].intensite);
					}
					else
					{
						pix_col = black + pix_col;
					}
				}
			}
			
			clamp255(pix_col);
			out << (int)pix_col.x << ' '
				<< (int)pix_col.y << ' '
				<< (int)pix_col.z << '\n';
		}
	}
}