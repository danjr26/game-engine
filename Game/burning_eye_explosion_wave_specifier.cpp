#include "burning_eye_explosion_wave_specifier.h"
#include "game_engine.h"

burning_eye::ExplosionWaveSpecifier::ExplosionWaveSpecifier() :
	mColorTransition() {

	mColorTransition.mKeys.push_back({ ColorRGBAf(1.0, 1.0, 1.0, 0.2), 0.5f });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.0, 0.0, 0.0, 0.0), 0.0 });
}

void burning_eye::ExplosionWaveSpecifier::init(ParticleSystem2& i_system, Accessor& i_accessor) {
	uint nParticles = 5;
	uint index = i_system.add(nParticles);
	i_system.access(index, i_accessor);
	for (uint i = 0; i < nParticles; i++) {
		i_accessor.clear(i);
		i_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		i_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(i_accessor, nParticles, i_system.getTransform(), i_system.getDepthTransform());
}

void burning_eye::ExplosionWaveSpecifier::update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mDimensions[i] += Vector2f(3.0, 3.0) * (float)i_dt;
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]);
	}
}

void burning_eye::ExplosionWaveSpecifier::destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount();) {
		if (i_accessor.mAge[i] > 0.5f) {
			i_system.remove(i);
		}
		else {
			i++;
		}
	}

	if (i_system.getCount() == 0) {
		GE.destruction().add(&i_system);
	}
}

void burning_eye::ExplosionWaveSpecifier::generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {}

