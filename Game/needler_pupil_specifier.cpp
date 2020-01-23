#include "needler_pupil_specifier.h"
#include "sphere.h"
#include "game_engine.h"

needler::PupilSpecifier::PupilSpecifier() :
	mGenerateStepper(0.005),
	mNGenerated(0) {

	mColorTransition.mKeys.assign({
		{ ColorRGBAf(1.0, 1.0, 1.0, 0.0), 0.05 },
		{ ColorRGBAf(1.0, 1.0, 1.0, 0.4), 0.4 },
		{ ColorRGBAf(1.0, 1.0, 1.0, 0.0), 0.05 }
		});
}

void needler::PupilSpecifier::init(ParticleSystem2 & i_system, Accessor & i_accessor) {}

void needler::PupilSpecifier::update(ParticleSystem2 & i_system, Accessor & i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mPosition[i] += i_accessor.mLinearVelocity[i] * (float)i_dt;
		i_accessor.mLinearVelocity->addToMagnitude(-60 * i_dt);
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]);
	}
}

void needler::PupilSpecifier::destroy(ParticleSystem2 & i_system, Accessor & i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount();) {
		if (i_accessor.mAge[i] > 0.5f) {
			i_system.remove(i);
		}
		else {
			i++;
		}
	}
}

void needler::PupilSpecifier::generate(ParticleSystem2 & i_system, Accessor & i_accessor, double i_dt) {
	if (i_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(i_dt);
	uint index = i_system.add(nToGenerate);
	i_system.access(index, i_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		i_accessor.mAge[i] = 0.0f;
		i_accessor.mPosition[i] = Vector3d(sin(GE.clock().now() * 4) * 5, 0, 0);
		i_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		i_accessor.mAngle[i] = geutil::random<uint>(3) * (2.0 * PI / 3.0);
		i_accessor.mLinearVelocity[i] = Vector3f(Vector2f(2, 0).rotated(-i_accessor.mAngle[i]));
		i_accessor.mUV1[i] = Vector2f(0, 0);
		i_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(i_accessor, nToGenerate, i_system.getTransform(), i_system.getDepthTransform());
}
