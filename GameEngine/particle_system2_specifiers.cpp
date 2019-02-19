#include "particle_system2_specifiers.h"

TestParticleSpecifier::TestParticleSpecifier() :
	mGenerateStepper(0.0005),
	mNGenerated(0),
	mColorTransition() {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.1, 0.2, 1.0, 1.0), 0.5 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.5), 0.5 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.0), 1.0 });
}

void TestParticleSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mPosition[i] -= Vector3f(300, 300, 0);
		in_accessor.mPosition[i].rotate(Vector3f(0, 0, 1), 0.5f * PI * in_dt);
		in_accessor.mPosition[i] += Vector3f(300, 300, 0);
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
	}
}

void TestParticleSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 1.0f) {
			in_system.remove(i);
		}
		else {
			i++;
		}
	}
}

void TestParticleSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	if (in_system.getCount() > 2000) return;
	uint nToGenerate = mGenerateStepper.stepNumber(in_dt);
	uint index = in_system.add(nToGenerate);
	in_system.access(index, in_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf(0.8, 0.1, 0.1, 1.0);
		in_accessor.mDimensions[i] = Vector2f(20, 20);
		in_accessor.mLinearVelocity[i] = Vector3f(200.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)));
		in_accessor.mPosition[i] = Vector3f(Vector2f(300, 300) + 50.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)));
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
}

AmbientDustSpecifier::AmbientDustSpecifier() :
	mColorTransition() {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.5, 1.0), 2.0 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.6, 1.0), 0.0 });
}

void AmbientDustSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		//in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
	}
}

void AmbientDustSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	return;
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 10.0f) {
			in_system.remove(i);
		}
		else {
			i++;
		}
	}
}

void AmbientDustSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	if (in_system.getCount() > 100) return;
	uint nToGenerate = 100 - in_system.getCount();
	uint index = in_system.add(nToGenerate);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf(0.5, 0.5, 0.6, 1.0);
		in_accessor.mDimensions[i] = Vector2f(0.1, 0.1);
		in_accessor.mLinearVelocity[i] = Vector3f(0.1 * Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)));
		in_accessor.mPosition[i] = Vector3f();
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
}
