#include "needler_iris_specifier.h"
#include "sphere.h"
#include "game_engine.h"

needler::IrisSpecifier::IrisSpecifier() :
	mGenerateStepper(0.01),
	mNGenerated(0) {

	mColorTransition.mKeys.assign({
		{ ColorRGBAf(0.4, 0.4, 1, 0.0), 0.05 },
		{ ColorRGBAf(1, 0.8, 1, 0.2), 0.4 },
		{ ColorRGBAf(1, 0.4, 0.4, 0.0), 0.05 }
		});
}

void needler::IrisSpecifier::init(ParticleSystem2 & i_system, Accessor & i_accessor) {}

void needler::IrisSpecifier::update(ParticleSystem2 & i_system, Accessor & i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mPosition[i] += i_accessor.mLinearVelocity[i] * (float)i_dt;
		i_accessor.mLinearVelocity->addToMagnitude(-60 * i_dt);
		i_accessor.mDimensions[i] = (
			Vector2f(1.2, 1.2) -
			Vector2f(2.4, 2.4) * i_accessor.mAge[i]
			);
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]);
	}
}

void needler::IrisSpecifier::destroy(ParticleSystem2 & i_system, Accessor & i_accessor, double i_dt) {
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

void needler::IrisSpecifier::generate(ParticleSystem2 & i_system, Accessor & i_accessor, double i_dt) {
	if (i_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(i_dt);
	uint index = i_system.add(nToGenerate);
	i_system.access(index, i_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		i_accessor.mAge[i] = 0.0f;
		i_accessor.mPosition[i] = Vector3d(sin(GE.clock().now() * 4) * 5, 0, 0);
		i_accessor.mAngle[i] = geutil::random<double>(-PI, PI);
		i_accessor.mAngularVelocity[i] = geutil::random<double>(-PI, PI) * 0.5;
		i_accessor.mLinearVelocity[i] = Vector3f(Vector2f(sqrt(geutil::random<double>(20)), 0).rotated(-i_accessor.mAngle[i]));
		i_accessor.mUV1[i] = Vector2f(0, 0);
		i_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(i_accessor, nToGenerate, i_system.getTransform(), i_system.getDepthTransform());
}
