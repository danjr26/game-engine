#include "particle_system2_specifiers.h"

TestParticleSpecifier::TestParticleSpecifier() :
	mGenerateStepper(0.0005),
	mNGenerated(0),
	mColorTransition() {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.1, 0.2, 1.0, 1.0), 0.5 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.5), 0.5 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.0), 1.0 });
}

void TestParticleSpecifier::Update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	//in_system.Reserve(6000);
	in_system.Access(0, in_accessor);
	for (uint i = 0; i < in_system.Count(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mPosition[i] -= Vector3f(300, 300, 0);
		in_accessor.mPosition[i].Rotate(Vector3f(0, 0, 1), 0.5f * PI * in_dt);
		in_accessor.mPosition[i] += Vector3f(300, 300, 0);
		in_accessor.mColor[i] = mColorTransition.Evaluate(in_accessor.mAge[i]);
	}
}

void TestParticleSpecifier::Destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.Access(0, in_accessor);
	for (uint i = 0; i < in_system.Count();) {
		if (in_accessor.mAge[i] > 1.0f) {
			in_system.Remove(i);
		}
		else {
			i++;
		}
	}
}

void TestParticleSpecifier::Generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	if (in_system.Count() > 2000) return;
	uint nToGenerate = mGenerateStepper.Step_Number(in_dt);
	uint index = in_system.Add(nToGenerate);
	in_system.Access(index, in_accessor);
	mNGenerated += nToGenerate;
	for (uint i = 0; i < nToGenerate; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf(0.8, 0.1, 0.1, 1.0);
		in_accessor.mDimensions[i] = Vector2f(20, 20);
		in_accessor.mLinearVelocity[i] = Vector3f(200.0 * Vector2f(sqrt(Random<double>(1)), 0).Rotated(Random<double>(2 * PI)));
		in_accessor.mPosition[i] = Vector3f(Vector2f(300, 300) + 50.0 * Vector2f(sqrt(Random<double>(1)), 0).Rotated(Random<double>(2 * PI)));
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
}
