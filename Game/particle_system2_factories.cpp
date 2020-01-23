#include "particle_system2_factories.h"
#include "particle_system2_specifiers.h"
#include "game.h"
#include "game_engine.h"
#include "burning_eye_explosion_smoke_specifier.h"
#include "burning_eye_explosion_wave_specifier.h"

void createBurningEyeExplosion(const Vector3d& i_position) {
	createSimpleParticleSystem2<burning_eye::ExplosionSmokeSpecifier>(i_position, "SmokeTexture");
	createSimpleParticleSystem2<burning_eye::ExplosionWaveSpecifier>(i_position, "RingWaveTexture");
}
