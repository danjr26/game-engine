#include "burning_eye_explosion_wave_specifier.h"
#include "game_engine.h"

burning_eye::ExplosionWaveSpecifier::ExplosionWaveSpecifier() :
	mColorTransition() {

	mColorTransition.mKeys.push_back({ ColorRGBAf(1.0, 1.0, 1.0, 0.2), 0.5f });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.0, 0.0, 0.0, 0.0), 0.0 });
}

void burning_eye::ExplosionWaveSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	uint nParticles = 5;
	uint index = in_system.add(nParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < nParticles; i++) {
		in_accessor.clear(i);
		in_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, nParticles, in_system.getTransform(), in_system.getDepthTransform());
}

void burning_eye::ExplosionWaveSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mDimensions[i] += Vector2f(3.0, 3.0) * (float)in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
	}
}

void burning_eye::ExplosionWaveSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 0.5f) {
			in_system.remove(i);
		}
		else {
			i++;
		}
	}

	if (in_system.getCount() == 0) {
		GE.destruction().add(&in_system);
	}
}

void burning_eye::ExplosionWaveSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}

