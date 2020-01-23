#include "burning_eye_iris_specifier.h"

burning_eye::IrisSpecifier::IrisSpecifier() :
	mGenerateStepper(0.003),
	mNGenerated(0) {

	mColorTransition.mKeys.assign({
		{ ColorRGBAf(0.9, 0.7, 0.4, 0.05), 0.10 },
		{ ColorRGBAf(0.7, 0.5, 0.1, 0.05), 0.25 },
		{ ColorRGBAf(0.5, 0.2, 0.1, 0.01), 0.15 },
		{ ColorRGBAf(0.2, 0.2, 0.1, 0.0), 0.0 }
		});
}

void burning_eye::IrisSpecifier::init(ParticleSystem2& i_system, Accessor& i_accessor) {}

void burning_eye::IrisSpecifier::update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mPosition[i] += i_accessor.mLinearVelocity[i] * (float)i_dt;
		i_accessor.mDimensions[i] = (Vector2f(0.2, 0.2) + Vector2f(0.1, 0.1) * i_accessor.mAge[i]) * 5.0;
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]);
		i_accessor.mAngle[i] += i_accessor.mAngularVelocity[i] * (float)i_dt;
	}
}

void burning_eye::IrisSpecifier::destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
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

void burning_eye::IrisSpecifier::generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	if (i_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(i_dt);
	uint index = i_system.add(nToGenerate);
	i_system.access(index, i_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		i_accessor.mAge[i] = 0.0f;
		i_accessor.mPosition[i] = Vector3d();
		i_accessor.mAngle[i] = geutil::random<double>(-PI, PI);
		i_accessor.mAngularVelocity[i] = geutil::random<double>(-PI, PI) * 0.5;
		i_accessor.mLinearVelocity[i] = Vector3f(0.06 * Vector2f(sqrt(geutil::random<double>(1)), 0).rotated(geutil::random<double>(2 * PI))) * 5.0;
		i_accessor.mUV1[i] = Vector2f(0, 0);
		i_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(i_accessor, nToGenerate, i_system.getTransform(), i_system.getDepthTransform());
}