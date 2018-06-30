#ifndef RANDOM_H
#define RANDOM_H
#include "vector.h"

int		Rand_Int	(int floor, int ceiling);
float	Rand_Float	(float floor, float ceiling);
double	Rand_Double	(double floor, double ceiling);

class PerlinNoise2f {
public:
	Vector2f*	nodes;
	int			x, y;

	PerlinNoise2f(int x, int y);
	~PerlinNoise2f();
	float Get(Vector2f v);
};

class PerlinNoise3f {
public:
	Vector3f*	nodes;
	int			x, y, z;

	PerlinNoise3f(int x, int y, int z);
	~PerlinNoise3f();
	float Get(Vector3f v);
};

#endif
