#include "particle_system2_specifiers.h"
#include "game_engine.h"
#include "basic_collision_mask.h"
#include "in_place_collision_evaluator.h"

BurningEyePupilSpecifier::BurningEyePupilSpecifier() :
	mGenerateStepper(0.002),
	mNGenerated(0) {

	mColorTransition.mKeys.assign({
		{ ColorRGBAf(1, 1, 1, 0.02), 0.5 },
		{ ColorRGBAf(1, 1, 1, 0.0), 0.0 }
		});
}

void BurningEyePupilSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) 
{}

void BurningEyePupilSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * (float)in_dt;
		in_accessor.mDimensions[i] = (Vector2f(0.06, 0.06) - Vector2f(0.04, 0.04) * in_accessor.mAge[i]) * 5.0;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
		in_accessor.mAngle[i] += in_accessor.mAngularVelocity[i] * (float)in_dt;
	}
}

void BurningEyePupilSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
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

void BurningEyePupilSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
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

AmbientDustSpecifier::AmbientDustSpecifier() :
	mColorTransition(),
	mMaxParticles(100) {
	
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
		in_accessor.mDimensions[i] = Vector2f(0.3, 0.3);
		in_accessor.mLinearVelocity[i] = Vector3f(1.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0).rotated(GEUtil::random<double>(2 * PI)));
		in_accessor.mPosition[i] = Vector3f(Vector2f(camRect.randomPointInside()), 0);
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, mMaxParticles, in_system.getTransform(), in_system.getDepthTransform());
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
	ParticleSystem2::transformParticles(in_accessor, in_system.getCount(), in_system.getTransform(), in_system.getDepthTransform());
}

void AmbientDustSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) 
{}

void AmbientDustSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) 
{}

TestBulletImpactSparksSpecifier::TestBulletImpactSparksSpecifier() :
	mColorTransition(),
	mMaxParticles(10) {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 1.0, 0.7), 0.3 });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.5, 0.5, 0.5, 0.0), 0.0 });
}

void TestBulletImpactSparksSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	uint index = in_system.add(mMaxParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < mMaxParticles; i++) {
		in_accessor.mAge[i] = 0.0f;
		in_accessor.mAngle[i] = 0.0f;
		in_accessor.mAngularVelocity[i] = 0.0f;
		in_accessor.mColor[i] = ColorRGBAf();
		in_accessor.mDimensions[i] = Vector2f(1.0, 1.0);
		in_accessor.mLinearVelocity[i] = Vector3f(2.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0)
			.rotated(GEUtil::random<double>(2 * PI)), 0.0);
		in_accessor.mPosition[i] = Vector3f(0, 0, 0.5);
		in_accessor.mUV1[i] = Vector2f(0, 0);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, mMaxParticles, in_system.getTransform(), in_system.getDepthTransform());
}

void TestBulletImpactSparksSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		float v = GEUtil::random<float>(-0.5, 0.5);
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]) + ColorRGBAf(v, v, v, 0);
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

BurningEyeExplosionSmokeSpecifier::BurningEyeExplosionSmokeSpecifier() :
	mColorTransition() {
	
	mColorTransition.mKeys.push_back({ ColorRGBAf(1.0, 1.0, 1.0, 0.4), 0.3 * 8.0f});
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.0, 0.0, 0.0, 0.0), 0.0 });
}

void BurningEyeExplosionSmokeSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	uint nParticles = 5;
	uint index = in_system.add(nParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < nParticles; i++) {
		in_accessor.clear(i);
		in_accessor.mPosition[i] = Vector3f(0.3 * Vector2f(sqrt(GEUtil::random<double>(1)), 0)
			.rotated(GEUtil::random<double>(2 * PI)), 0.0);
		in_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		in_accessor.mUV2[i] = Vector2f(1, 1);
		in_accessor.mAngle[i] = GEUtil::random<double>(0.0, 2 * PI);
		in_accessor.mAngularVelocity[i] = GEUtil::random<double>(-PI/4, PI/4);
		in_accessor.mLinearVelocity[i] = Vector3f(3.0 * Vector2f(sqrt(GEUtil::random<double>(1)), 0)
			.rotated(GEUtil::random<double>(2 * PI)), 0.0);
	}
	ParticleSystem2::transformParticles(in_accessor, nParticles, in_system.getTransform(), in_system.getDepthTransform());
}

void BurningEyeExplosionSmokeSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mDimensions[i] += Vector2f(10.0, 10.0) * (float)in_dt / 8.0f;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
		in_accessor.mAngle[i] += in_accessor.mAngularVelocity[i] * (float)in_dt / 8.0f;
		in_accessor.mPosition[i] += in_accessor.mLinearVelocity[i] * (float)in_dt / 8.0f;
	}
}

void BurningEyeExplosionSmokeSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 0.3f * 8.0f) {
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

void BurningEyeExplosionSmokeSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {}

BurningEyeExplosionWaveSpecifier::BurningEyeExplosionWaveSpecifier() :
	mColorTransition() {

	mColorTransition.mKeys.push_back({ ColorRGBAf(1.0, 1.0, 1.0, 0.2), 0.5f });
	mColorTransition.mKeys.push_back({ ColorRGBAf(0.0, 0.0, 0.0, 0.0), 0.0 });
}

void BurningEyeExplosionWaveSpecifier::init(ParticleSystem2& in_system, Accessor& in_accessor) {
	uint nParticles = 5;
	uint index = in_system.add(nParticles);
	in_system.access(index, in_accessor);
	for (uint i = 0; i < nParticles; i++) {
		in_accessor.clear(i);
		in_accessor.mDimensions[i] = Vector2f(0.2, 0.2);
		in_accessor.mUV2[i] = Vector2f(1, 1);
	}
	ParticleSystem2::transformParticles(in_accessor, nParticles, in_system.getTransform(), in_system.getDepthTransform());
}

void BurningEyeExplosionWaveSpecifier::update(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount(); i++) {
		in_accessor.mAge[i] += (float)in_dt;
		in_accessor.mDimensions[i] += Vector2f(3.0, 3.0) * (float)in_dt;
		in_accessor.mColor[i] = mColorTransition.evaluate(in_accessor.mAge[i]);
	}
}

void BurningEyeExplosionWaveSpecifier::destroy(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) {
	in_system.access(0, in_accessor);
	for (uint i = 0; i < in_system.getCount();) {
		if (in_accessor.mAge[i] > 0.5f) {
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

void BurningEyeExplosionWaveSpecifier::generate(ParticleSystem2& in_system, Accessor& in_accessor, double in_dt) 
{}

