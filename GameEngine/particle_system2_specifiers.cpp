#include "particle_system2_specifiers.h"
#include "game_engine.h"
#include "basic_collision_mask.h"
#include "in_place_collision_evaluator.h"

TestParticleSpecifier::TestParticleSpecifier() :
	mGenerateStepper(0.0005),
	mNGenerated(0),
	mColorTransition() {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.1, 0.2, 1.0, 1.0), 0.5 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.5), 0.5 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.7, 0.5, 0.1, 0.0), 1.0 });
}

void TestParticleSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {}

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
	mColorTransition(),
	mMaxParticles(100) {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.5, 0.0), 0.0 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.6, 1.0), 0.0 });
}

void AmbientDustSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	Rectangled camRect = GE.cameras().get(CameraManager::ID::main)->getTransformedRectangle();

	in_system.access(0, in_accessor);
	uint index = in_system.add(mMaxParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < mMaxParticles; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf(0.5, 0.5, 0.6, 1.0);
		in_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		in_accessor.mLinearVelocity[i] = Vector3f(1.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)));
		in_accessor.mPosition[i] = Vector3f(Vector2f(camRect.randomPointInside()), 0);
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
}

void AmbientDustSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	Rectangled camRect = GE.cameras().get(CameraManager::ID::main)->getTransformedRectangle();
	Vector2d axes[2];
	camRect.getAxes(axes);
	double dots[2] = {
		axes[0].dotSelf(),
		axes[1].dotSelf()
	};

	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);

		Vector2d fromOrigin = Vector2f(in_accessor.mPosition[i]) - camRect.getOrigin();
		for (uint j = 0; j < 2; j++) {
			double dot = fromOrigin.dot(axes[j]);
			if (dot < 0) {
				in_accessor.mPosition[i] += Vector3d(axes[j], 0);
			}
			else if (dot > dots[j]) {
				in_accessor.mPosition[i] -= Vector3d(axes[j], 0);
			}
		}
	}
}

void AmbientDustSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) 
{}

void AmbientDustSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) 
{}

TestBulletImpactSparksSpecifier::TestBulletImpactSparksSpecifier() :
	mColorTransition(),
	mMaxParticles(10) {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 1.0, 0.5), 0.3 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.5, 0.0), 0.0 });
}

void TestBulletImpactSparksSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	in_system.access(0, in_accessor);
	uint index = in_system.add(mMaxParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < mMaxParticles; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf();
		in_accessor.mDimensions[i] = Vector2f(0.5, 0.5);
		in_accessor.mLinearVelocity[i] = Vector3f(2.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)), 0.0);
		in_accessor.mPosition[i] = Vector3f(0, 0, 0.5);
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
}

void TestBulletImpactSparksSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
	}
}

void TestBulletImpactSparksSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 0.3f) {
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

void TestBulletImpactSparksSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}
