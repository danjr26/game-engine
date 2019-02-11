#include "asteroid.h"
#include "framework.h"
/*
Asteroid::Asteroid(Mesh* meshData, Transform3d transform) :
Entity(transform, Identity::asteroid, Allegiance::neutral, meshData->tree, 0.016),
meshData(meshData) {
	mesh = new MeshInstance(meshData);
	mesh->transform = transform;
}

bool Asteroid::Collide(CollidableComponent3* that, Collision* collision) {
	return false;
}

Mesh* Asteroid::Random(int detail, float minr, float maxr) {
	RawMesh icosphere = Ico_Sphere(detail, 1.0, true, true);

	PerlinNoise3f noise = PerlinNoise3f(2, 2, 2);
	PerlinNoise3f noise2 = PerlinNoise3f(4, 4, 4);
	
	for (int i = 0; i < icosphere.ntris * 3; i++) {
		Vector3f nvert = (icosphere.vertices[i] + Vector3f(1, 1, 1)) * 0.5;
		icosphere.vertices[i] = icosphere.vertices[i] * minr +
			icosphere.vertices[i] * (maxr - minr) * fabs(noise.Get(nvert)) * 0.75 +
			icosphere.vertices[i] * (maxr - minr) * fabs(noise2.Get(nvert)) * 0.25;
	}

	Vector3f* facenorms = new Vector3f[icosphere.ntris * 3];
	for (int i = 0; i < icosphere.ntris * 3; i += 3) {
		facenorms[i] = (icosphere.vertices[i + 1] - icosphere.vertices[i]).Normalized().Cross
			((icosphere.vertices[i + 2] - icosphere.vertices[i]).Normalized());
		if (facenorms[i].Dot(icosphere.vertices[i]) < 0.0f)
			facenorms[i] = facenorms[i] * -1;
		facenorms[i + 1] = facenorms[i];
		facenorms[i + 2] = facenorms[i];
	}

	for (int i = 0; i < icosphere.ntris * 3; i++) {
		Vector3f tot = facenorms[i];
		int count = 1;
		for (int j = 0; j < icosphere.ntris * 3; j++) {
			if (icosphere.vertices[i] == icosphere.vertices[j]) {
				tot += facenorms[j];
				count++;
			}
		}
		icosphere.norms[i] = tot / (float)count;
	}

	delete[] facenorms;

	Material matl;
	matl.ambient = Color4f(0.05f, 0.05f, 0.05f, 1.0f);
	matl.diffuse = Color4f(0.5f, 0.25f, 0.1f, 1.0f);
	matl.texture = (Texture2*)GEngine::Get().Resource().Get_Resource("rock.tga");

	Mesh* mesh = new Mesh("", "asteroid");
	mesh->Load_Raw(icosphere.vertices, icosphere.norms, icosphere.texcs, icosphere.ntris, matl);
	return mesh;
}
*/