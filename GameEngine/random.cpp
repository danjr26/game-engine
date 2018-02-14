#include "random.h"
#include "framework.h"

int Rand_Int(int floor, int ceiling) {
	return rand() / RAND_MAX * (ceiling - floor) + floor;
}

float Rand_Float(float floor, float ceiling) {
	return rand() / (float)RAND_MAX * (ceiling - floor) + floor;
}

double Rand_Double(double floor, double ceiling) {
	return rand() / (double)RAND_MAX * (ceiling - floor) + floor;
}

PerlinNoise2f::PerlinNoise2f(int x, int y)
: x(x), y(y) {
	nodes = new Vector2f[x * y];
	for (int i = 0; i < x * y; i++)
		nodes[i] = Vector2f::Random_Normalized();
	for (int i = 0; i < x; i++)
		nodes[x * (y - 1) + i] = nodes[i];
	for (int i = 0; i < y; i++)
		nodes[x * i + (x - 1)] = nodes[x * i];
}

PerlinNoise2f::~PerlinNoise2f() {
	delete[] nodes;
}

float PerlinNoise2f::Get(Vector2f v) {
	if (x == 0 || y == 0)
		exit(-1);
	int vx = (int)(v.x * (x - 1));
	int vy = (int)(v.y * (y - 1));
	Vector2f relv = Vector2f(
		((v.x * (x - 1)) - vx),
		((v.y * (y - 1)) - vy)
		);
	if (vx == x - 1)
		vx = x - 2;
	if (vy == y - 1)
		vy = y - 2;
	Vector2f corners[4] = {
		Vector2f((float)vx / x, (float)vy / y),
		Vector2f((float)vx / x, (float)(vy + 1) / y),
		Vector2f((float)(vx + 1) / x, (float)(vy + 1) / y),
		Vector2f((float)(vx + 1) / x, (float)vy / y)
	};
	Vector2f cornernodes[4] = {
		nodes[vy * x + vx],
		nodes[(vy + 1) * x + vx],
		nodes[(vy + 1) * x + vx + 1],
		nodes[vy * x + vx + 1]
	};

	float dots[4];
	for (int i = 0; i < 4; i++)
		dots[i] = (v - corners[i]).Normalized().Dot(cornernodes[i]);

	float weights[4] = {
		dots[0] * (1.0f - relv.x) * (1.0f - relv.y),
		dots[1] * (1.0f - relv.x) * (relv.y),
		dots[2] * (relv.x) * (relv.y),
		dots[3] * (relv.x) * (1.0f - relv.y)
	};

	float total = 0.0f;
	for (int i = 0; i < 4; i++)
		total += weights[i];

	if (total != total)
		exit(-1);

	return total;
}

PerlinNoise3f::PerlinNoise3f(int x, int y, int z)
	: x(x), y(y), z(z) {
	nodes = new Vector3f[x * y * z];
	for (int i = 0; i < x * y * z; i++)
		nodes[i] = Vector3f::Random_Normalized();
}

PerlinNoise3f::~PerlinNoise3f() {
	delete[] nodes;
}

float PerlinNoise3f::Get(Vector3f v) {
	if (x == 0 || y == 0 || z == 0)
		exit(-1);
	int vx = (int)(v.x * (x - 1));
	int vy = (int)(v.y * (y - 1));
	int vz = (int)(v.z * (z - 1));
	Vector3f relv = Vector3f(
		((v.x * (x - 1)) - vx),
		((v.y * (y - 1)) - vy),
		((v.z * (z - 1)) - vz)
		);
	if (vx == x - 1)
		vx = x - 2;
	if (vy == y - 1)
		vy = y - 2;
	if (vz == z - 1)
		vz = z - 2;
	Vector3f corners[8] = {
		Vector3f((float)vx / x, (float)vy / y, (float)vz / z),
		Vector3f((float)vx / x, (float)vy / y, (float)(vz + 1) / z),
		Vector3f((float)vx / x, (float)(vy + 1) / y, (float)vz / z),
		Vector3f((float)vx / x, (float)(vy + 1) / y, (float) (vz + 1) / z),
		Vector3f((float)(vx + 1) / x, (float)vy / y, (float)vz / z),
		Vector3f((float)(vx + 1) / x, (float)vy / y, (float)(vz + 1) / z),
		Vector3f((float)(vx + 1) / x, (float)(vy + 1) / y, (float)vz / z),
		Vector3f((float)(vx + 1) / x, (float)(vy + 1) / y, (float)(vz + 1) / z)
	};
	Vector3f cornernodes[8] = {
		nodes[vz * y * x + vy * x + vx],
		nodes[vz * y * x + vy * x + vx + 1],
		nodes[vz * y * x + (vy + 1) * x + vx],
		nodes[vz * y * x + (vy + 1) * x + vx + 1],
		nodes[(vz + 1) * y * x + vy * x + vx],
		nodes[(vz + 1) * y * x + vy * x + vx + 1],
		nodes[(vz + 1) * y * x + (vy + 1) * x + vx],
		nodes[(vz + 1) * y * x + (vy + 1) * x + vx + 1],
	};

	float dots[8];
	for (int i = 0; i < 8; i++)
		dots[i] = (v - corners[i]).Normalized().Dot(cornernodes[i]);

	float weights[8] = {
		dots[0] * (1.0f - relv.x) * (1.0f - relv.y) * (1.0f - relv.z),
		dots[1] * (relv.x) * (1.0f - relv.y) * (1.0f - relv.z),
		dots[2] * (1.0f - relv.x) * (relv.y) * (1.0f - relv.z),
		dots[3] * (relv.x) * (relv.y) * (1.0f - relv.z),
		dots[4] * (1.0f - relv.x) * (1.0f - relv.y) * (relv.z),
		dots[5] * (relv.x) * (1.0f - relv.y) * (relv.z),
		dots[6] * (1.0f - relv.x) * (relv.y) * (relv.z),
		dots[7] * (relv.x) * (relv.y) * (relv.z),
	};

	float total = 0.0f;
	for (int i = 0; i < 8; i++)
		total += weights[i];

	if (total != total || isinf(total))
		exit(-1);

	return total;
}

