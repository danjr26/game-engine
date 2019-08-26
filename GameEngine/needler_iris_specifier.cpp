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

void needler::IrisSpecifier::init(ParticleSystem2 & in_system, Accessor & in_accessor) {}

void needler::IrisSpecifier::update(ParticleSystem2 & in_system, Accessor & in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * (float)in_dt;
		in_accessor.mLinearVelocity->addToMagnitude(-60 * in_dt);
		in_accessor.mDimensions[i] = (
			Vector2f(1.2, 1.2) -
			Vector2f(2.4, 2.4) * in_accessor.mAge[i]
			);
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
	}
}

void needler::IrisSpecifier::destroy(ParticleSystem2 & in_system, Accessor & in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 0.5f) {
			in_system.remove(i);
		}
		else {
			i++;
		}
	}
}

void needler::IrisSpecifier::generate(ParticleSystem2 & in_system, Accessor & in_accessor, double in_dt) {
	if (in_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(in_dt);
	uint index = in_system.add(nToGenerate);
	in_system.access(index, in_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mPosition[i] = Vector3d(sin(GE.clock().now() * 4) * 5, 0, 0);
		in_accessor.mAngle[i] = GEUtil::random<double>(-PI, PI);
		in_accessor.mAngularVelocity[i] = GEUtil::random<double>(-PI, PI) * 0.5;
		in_accessor.mLinearVelocity[i] = Vector3f(Vector2f(sqrt(GEUtil::random<double>(20)), 0).rotated(-in_accessor.mAngle[i]));
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, nToGenerate, in_system.getTransform(), in_system.getDepthTransform());
}
