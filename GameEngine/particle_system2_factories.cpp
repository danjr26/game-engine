#include "particle_system2_factories.h"
#include "particle_system2_specifiers.h"
#include "game.h"
#include "game_engine.h"

void createTestEnemyExplosion(const Vector3d& in_position) {
	createSimpleParticleSystem2<TestEnemyExplosionSmokeSpecifier>(in_position, "SmokeTexture");
	createSimpleParticleSystem2<TestEnemyExplosionWaveSpecifier>(in_position, "RingWaveTexture");
}
