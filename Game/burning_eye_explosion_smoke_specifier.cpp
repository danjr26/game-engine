#include "burning_eye_explosion_smoke_specifier.h"
#include "game_engine.h"

burning_eye::ExplosionSmokeSpecifier::ExplosionSmokeSpecifier() :
	mColorTransition() {

	mColorTransition.mKeys.push_back({ ColorRGBAf(1.0, 1.0, 1.0, 0.4), 0.3 * 8.0f });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.0, 0.0, 0.0, 0.0), 0.0 });
}

void burning_eye::ExplosionSmokeSpecifier::init(ParticleSystem2& i_system, Accessor& i_accessor) {
	uint nParticles = 5;
	uint index = i_system.add(nParticles);
	i_system.access(index, i_accessor);
	for (uint i = 0; i < nParticles; i++) {
		i_accessor.clear(i);
		i_accessor.mPosition[i] = Vector3f(0.3 * Vector2f(sqrt(geutil::random<double>(1)), 0)
			.rotated(geutil::random<double>(2 * PI)), 0.0);
		i_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		i_accessor.mUV2[i] = Vector2f(1, 1);
		i_accessor.mAngle[i] = geutil::random<double>(0.0, 2 * PI);
		i_accessor.mAngularVelocity[i] = geutil::random<double>(-PI / 4, PI / 4);
		i_accessor.mLinearVelocity[i] = Vector3f(3.0 * Vector2f(sqrt(geutil::random<double>(1)), 0)
			.rotated(geutil::random<double>(2 * PI)), 0.0);
	}
	ParticleSystem2::transformParticles(i_accessor, nParticles, i_system.getTransform(), i_system.getDepthTransform());
}

void burning_eye::ExplosionSmokeSpecifier::update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mDimensions[i] += Vector2f(10.0, 10.0) * (float)i_dt / 8.0f;
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]);
		i_accessor.mAngle[i] += i_accessor.mAngularVelocity[i] * (float)i_dt / 8.0f;
		i_accessor.mPosition[i] += i_accessor.mLinearVelocity[i] * (float)i_dt / 8.0f;
	}
}

void burning_eye::ExplosionSmokeSpecifier::destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount();) {
		if (i_accessor.mAge[i] > 0.3f * 8.0f) {
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

void burning_eye::ExplosionSmokeSpecifier::generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {}
