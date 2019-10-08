#pragma once
#include <iostream>

template<typename T>
struct Vec3 {
	T x, y, z;
};


//Operateur egalitaire
template<typename T>
bool operator==(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
};

//Operateur multiplicateur entre 2 vec3
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x* b.x, a.y* b.y, a.z* b.z };
};

//Operateur multiplicateur avec un scalaire
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, double b) {
	return Vec3<T>{ a.x* b, a.y* b, a.z* b };
};


//Operateur diviseur entre 2 vec3 
template<typename T>
Vec3<T> operator/(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x / b.x, a.y / b.y, a.z / b.z };
};

//Operateur diviseur avec un scalaire
template<typename T>
Vec3<T> operator/(const Vec3<T>& a, double b) {
	return Vec3<T>{ a.x / b, a.y / b, a.z / b };
};

//Operateur diviseur avec un scalaire
template<typename T>
Vec3<T> operator/( double b, const Vec3<T>& a) {
	return Vec3<T>{ b / a.x, b / a.y, b/a.z };
};

//Operateur de soustraction
template<typename T>
Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x - b.x, a.y - b.y, a.z - b.z };
};

//Operateur de soustraction
template<typename T>
Vec3<T> operator-(const Vec3<T>& a, double b) {
	return Vec3<T>{ a.x - b, a.y - b, a.z - b };
};

//Operateur d'addition
template<typename T>
Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x + b.x, a.y + b.y, a.z + b.z };
};

//Operateur d'addition avec un scalaire
template<typename T>
Vec3<T> operator+(const Vec3<T>& a, double b) {
	return Vec3<T>{ a.x + b, a.y + b, a.z + b };
};



//Calcul de la nomre d'un vecteur
template<typename T>
double norm(const Vec3<T>& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
};

template<typename T>
Vec3<T> normalize(const Vec3<T>& v) {
	double norme = norm(v);
	return Vec3<T>{ v.x / norme, v.y / norme, v.z / norme };
}


//Calcul du produit scalaire
template<typename T>
double dot(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
double dot2(const Vec3<T>& a) {
	return norm(a) * norm(a);
}

template<typename T>
void clamp255(Vec3<T>& col) {
	col.x = (col.x > 255) ? 255 : (col.x < 0) ? 0 : col.x;
	col.y = (col.y > 255) ? 255 : (col.y < 0) ? 0 : col.y;
	col.z = (col.z > 255) ? 255 : (col.z < 0) ? 0 : col.z;
}

template<typename T>
void debugLog(const Vec3<T>& v) {
	std::cout << "x : " << v.x << " y : " << v.y << " z : " << v.z << std::endl;
}

float random_between_two_float(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}

