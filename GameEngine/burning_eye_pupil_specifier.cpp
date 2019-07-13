#include "burning_eye_pupil_specifier.h"
#include "sphere.h"

burning_eye::PupilSpecifier::PupilSpecifier() :
	mGenerateStepper(0.002),
	mNGenerated(0) {

	mColorTransition.mKeys.assign({
		{ ColorRGBAf(1, 1, 1, 0.02), 0.5 },
		{ ColorRGBAf(1, 1, 1, 0.0), 0.0 }
		});
}

void burning_eye::PupilSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {}

void burning_eye::PupilSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * (float)in_dt;
		in_accessor.mDimensions[i] = (Vector2f(0.06, 0.06) - Vector2f(0.04, 0.04) * in_accessor.mAge[i]) * 5.0;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
		in_accessor.mAngle[i] += in_accessor.mAngularVelocity[i] * (float)in_dt;
	}
}

void burning_eye::PupilSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
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

void burning_eye::PupilSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	if (in_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(in_dt);
	uint index = in_system.add(nToGenerate);
	in_system.access(index, in_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mPosition[i] = Vector3d();
		in_accessor.mPosition[i] += Vector3d(Circled::fromPointRadius(Vector2d(), 0.1).randomPointInside(), -0.01);
		in_accessor.mAngle[i] = GEUtil::random<double>(-PI, PI);
		in_accessor.mAngularVelocity[i] = GEUtil::random<double>(-PI, PI) * 0.5;
		in_accessor.mLinearVelocity[i] = Vector3f(Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)));
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, nToGenerate, in_system.getTransform(), in_system.getDepthTransform());
}