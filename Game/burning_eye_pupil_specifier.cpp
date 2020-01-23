#include "burning_eye_pupil_specifier.h"
#include "sphere.h"

burning_eye::PupilSpecifier::PupilSpecifier() :
	mGenerateStepper(0.01),
	mNGenerated(0) {

	mColorTransition.mKeys.assign({
		{ ColorRGBAf(1, 1, 1, 0.1), 0.5 },
		{ ColorRGBAf(1, 1, 1, 0.0), 0.0 }
		});
}

void burning_eye::PupilSpecifier::init(ParticleSystem2& i_system, Accessor& i_accessor) {}

void burning_eye::PupilSpecifier::update(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	i_system.access(0, i_accessor);
	for (uint i = 0; i < i_system.getCount(); i++) {
		i_accessor.mAge[i] += (float)i_dt;
		i_accessor.mPosition[i] += i_accessor.mLinearVelocity[i] * (float)i_dt;
		i_accessor.mDimensions[i] = (Vector2f(0.06, 0.06) - Vector2f(0.04, 0.04) * i_accessor.mAge[i]) * 5.0;
		i_accessor.mColor[i] = mColorTransition.evaluate(i_accessor.mAge[i]);
		i_accessor.mAngle[i] += i_accessor.mAngularVelocity[i] * (float)i_dt;
	}
}

void burning_eye::PupilSpecifier::destroy(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
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

void burning_eye::PupilSpecifier::generate(ParticleSystem2& i_system, Accessor& i_accessor, double i_dt) {
	if (i_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(i_dt);
	uint index = i_system.add(nToGenerate);
	i_system.access(index, i_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		i_accessor.mAge[i] = 0.0f;
		i_accessor.mPosition[i] = Vector3d();
		i_accessor.mPosition[i] += Vector3d(Circled::fromPointRadius(Vector2d(), 0.1).randomPointInside(), -0.01);
		i_accessor.mAngle[i] = geutil::random<double>(-PI, PI);
		i_accessor.mAngularVelocity[i] = geutil::random<double>(-PI, PI) * 0.5;
		i_accessor.mLinearVelocity[i] = Vector3f(Vector2f(sqrt(geutil::random<double>(1)), 0).rotated(geutil::random<double>(2 * PI)));
		i_accessor.mUV1[i] = Vector2f(0, 0);
		i_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(i_accessor, nToGenerate, i_system.getTransform(), i_system.getDepthTransform());
}