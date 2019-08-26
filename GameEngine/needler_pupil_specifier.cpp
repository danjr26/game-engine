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

void needler::PupilSpecifier::init(ParticleSystem2 & in_system, Accessor & in_accessor) {}

void needler::PupilSpecifier::update(ParticleSystem2 & in_system, Accessor & in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * (float)in_dt;
		in_accessor.mLinearVelocity->addToMagnitude(-60 * in_dt);
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
	}
}

void needler::PupilSpecifier::destroy(ParticleSystem2 & in_system, Accessor & in_accessor, double in_dt) {
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

void needler::PupilSpecifier::generate(ParticleSystem2 & in_system, Accessor & in_accessor, double in_dt) {
	if (in_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(in_dt);
	uint index = in_system.add(nToGenerate);
	in_system.access(index, in_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mPosition[i] = Vector3d(sin(GE.clock().now() * 4) * 5, 0, 0);
		in_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		in_accessor.mAngle[i] = GEUtil::random<uint>(3) * (2.0 * PI / 3.0);
		in_accessor.mLinearVelocity[i] = Vector3f(Vector2f(2, 0).rotated(-in_accessor.mAngle[i]));
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, nToGenerate, in_system.getTransform(), in_system.getDepthTransform());
}
