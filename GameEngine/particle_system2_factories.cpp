#include "particle_system2_factories.h"
#include "particle_system2_specifiers.h"
#include "game.h"
#include "game_engine.h"

void createBurningEyeExplosion(const Vector3d& in_position) {
	createSimpleParticleSystem2<BurningEyeExplosionSmokeSpecifier>(in_position, "SmokeTexture");
	createSimpleParticleSystem2<BurningEyeExplosionWaveSpecifier>(in_position, "RingWaveTexture");
}
