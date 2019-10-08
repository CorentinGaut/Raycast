#include <fstream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <optional>
#include "RayCast.h"


Vec3<double> pix_col(black);

//fonction d'intersection entre une sphere et un rayon de lumiere
std::optional<double> intersect(Sphere S, Ray L) {
	double a = 1/*dot2(L.direction)*/;
	double b = 2 * (dot(L.position, L.direction) - dot(S.position, L.direction));
	double c = dot(L.position, L.position) + dot(S.position, S.position) - (2 * dot(S.position, L.position)) - (S.rayon * S.rayon);

	double delta = (b * b) - (4 * a * c);

	if (delta >= 0) {
		delta = sqrt(delta);
		const double t0 = (-b - delta) / 2;
		const double t1 = (-b + delta) / 2;
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

Vec3<double> RayCast(Ray ray,int nbRebonds) {
	Vec3<double> couleur = black;
	Vec3<double> pix_col = black;
	int rb = nbRebonds + 1;
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
		//sphere miroir
		Vec3<double> posIntersection = ray.position + (ray.direction * (t_min.value() - 0.1));
		if (objetsScenes[index].albedo >= 1) {
			
			Vec3<double> normalIntersection = posIntersection - objetsScenes[index].position;
			Vec3<double> rebondMiroir = normalIntersection * 2 * dot(ray.direction * (-1), normalize(normalIntersection)) + ray.direction;

			Ray rebond{ posIntersection,
						rebondMiroir };

			return RayCast(rebond,rb);
		}

		Vec3<double> normalIntersection = normalize(posIntersection - objetsScenes[index].position);

		Vec3<double> randomdir{ random_between_two_float(-0.0001,0.0001),random_between_two_float(-0.0001, 0.0001),random_between_two_float(-0.0001, 0.0001) };
		Vec3<double> rebondMiroir = normalIntersection * 2 * dot(ray.direction * (-1), normalize(normalIntersection)) + ray.direction;
		Ray rebondLumiereIndirect = { posIntersection,
								      normalize(rebondMiroir ) };

		for (int l = 0; l < LumieresScenes.size(); l++) {
			for (int h = 0; h < nbLumieresSurface; h++) {

				//generation d'une lumiere aleatoire pour les lumieres surfaciques
				int randX = rand() % 20;
				int randY = rand() % 19;
				int randZ = rand() % 19;
				Lumiere lightSu{ Vec3<double>{LumieresScenes[l].position.x + randX,
											  LumieresScenes[l].position.y + randY,
											  LumieresScenes[l].position.z + randZ}, white, 5 };

				Vec3<double> DirectionLumiere = lightSu.position - posIntersection;
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
					couleur = couleur + objetsScenes[index].couleur * lightSu.couleur * dt / (norm(DirectionLumiere) * LumieresScenes.size() * nbLumieresSurface);
				}
				//test si une sphere a intersecter un objet derriere la lumiere 
				else if (rebondIntersecte_min.value() >= norm(DirectionLumiere))
				{
					couleur = couleur + objetsScenes[index].couleur * lightSu.couleur * dt / (norm(DirectionLumiere) * LumieresScenes.size() * nbLumieresSurface);
				}
				else
				{
					couleur = black + couleur;
				}
			}
		}
		if (rb < rebondMax) {
			pix_col = (couleur * (1 - 0.3)) + (RayCast(rebondLumiereIndirect, rb) * 0.8);
		}
	}

	return pix_col;
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
			Vec3<double> directionCamera = normalize(Vec3<double>{ (double)x, (double)y, 0 } -positionPerspective);

			Ray ray{
				Vec3<double>{(double)x, (double)y, 0},
				directionCamera
			};

			Vec3<double> col = RayCast(ray,0);

			clamp255(col);
			out << (int)col.x << ' '
				<< (int)col.y << ' '
				<< (int)col.z << '\n';
		}
	}
}