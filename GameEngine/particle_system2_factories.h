#ifndef PARTICLE_SYSTEM2_FACTORIES
#define PARTICLE_SYSTEM2_FACTORIES

#include "vector.h"
#include "game_engine.h"
#include <string>

template<class T>
void createSimpleParticleSystem2(const Vector3d& in_position, const std::string& in_texName) {
	Texture* tex = GE.assets().get<Texture>(in_texName);
	T* specifier = new T;
	ParticleSystem2* system = new ParticleSystem2(tex, specifier);
	system->getTransform().setLocalPosition(Vector2d(in_position));
	system->getDepthTransform().setLocalDepth(in_position.z());
	GE.perFrameUpdate().add(system);
	GE.render().add(system);
}

void createTestEnemyExplosion(const Vector3d& in_position);

#endif
