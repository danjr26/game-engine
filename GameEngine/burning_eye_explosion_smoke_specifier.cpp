#include "burning_eye_explosion_smoke_specifier.h"
#include "game_engine.h"

burning_eye::ExplosionSmokeSpecifier::ExplosionSmokeSpecifier() :
	mColorTransition() {

	mColorTransition.mKeys.push_back({ ColorRGBAf(1.0, 1.0, 1.0, 0.4), 0.3 * 8.0f });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.0, 0.0, 0.0, 0.0), 0.0 });
}

void burning_eye::ExplosionSmokeSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	uint nParticles = 5;
	uint index = in_system.add(nParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < nParticles; i++) {
		in_accessor.clear(i);
		in_accessor.mPosition[i] = Vector3f(0.3 * Vector2f(sqrt(GEUtil::random<double>(1)), 0)
			.rotated(GEUtil::random<double>(2 * PI)), 0.0);
		in_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		in_accessor.mUV2[i] = Vector2f(1, 1);
		in_accessor.mAngle[i] = GEUtil::random<double>(0.0, 2 * PI);
		in_accessor.mAngularVelocity[i] = GEUtil::random<double>(-PI / 4, PI / 4);
		in_accessor.mLinearVelocity[i] = Vector3f(3.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0)
			.rotated(GEUtil::random<double>(2 * PI)), 0.0);
	}
	ParticleSystem2::transformParticles(in_accessor, nParticles, in_system.getTransform(), in_system.getDepthTransform());
}

void burning_eye::ExplosionSmokeSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mDimensions[i] += Vector2f(10.0, 10.0) * (float)in_dt / 8.0f;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
		in_accessor.mAngle[i] += in_accessor.mAngularVelocity[i] * (float)in_dt / 8.0f;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * (float)in_dt / 8.0f;
	}
}

void burning_eye::ExplosionSmokeSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 0.3f * 8.0f) {
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

void burning_eye::ExplosionSmokeSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}