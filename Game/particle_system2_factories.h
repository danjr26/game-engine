#ifndef PARTICLE_SYSTEM2_FACTORIES
#define PARTICLE_SYSTEM2_FACTORIES

#include "vector.h"
#include "game_engine.h"
#include "particle_system2.h"
#include <string>

template<class T>
ParticleSystem2* createSimpleParticleSystem2(const Vector3d& i_position, const std::string& i_texName) {
	Texture* tex = GE.assets().get<Texture>(i_texName);
	T* specifier = new T;

	ParticleSystem2* system = new ParticleSystem2(tex, specifier, ParticleSystem2::StateFlags::suppress_init);
	system->getTransform().setLocalPosition(Vector2d(i_position));
	system->getDepthTransform().setLocalDepth(i_position.z());

	ParticleSystem2::Accessor accessor;
	system->access(0, accessor);
	system->getSpecifier()->init(*system, accessor);

	GE.perFrameUpdate().add(system);
	GE.render().add(system);
	return system;
}

void createBurningEyeExplosion(const Vector3d& i_position);

#endif
