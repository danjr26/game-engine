#include "burning_eye_renderer.h"
#include "burning_eye.h"
#include "burning_eye_iris.h"
#include "game_engine.h"
#include "particle_system2_factories.h"

BurningEyeRenderer::BurningEyeRenderer(BurningEye& in_parent) :
	mParent(in_parent),
	mSprite(Circled::fromPointRadius(Vector2d(0, 0), 0.5), ColorRGBAf(0.8, 0.2, 0.2, 1.0), 2.0f),
	mIris(nullptr),
	mPupil(nullptr) {

	mPupil = createSimpleParticleSystem2<BurningEyePupilSpecifier>(Vector3d(), "GlowTexture");
	mPupil->setBlendSettings(BlendSettings::subtractive());
	mPupil->getDepthTransform().setLocalDepth(0.01);

	mIris = createSimpleParticleSystem2<IrisSpecifier>(Vector3d(), "FlameTexture");
	mIris->setBlendSettings(BlendSettings::additive());

	//GE.render().add(&mSprite);
}

BurningEyeRenderer::~BurningEyeRenderer() {
	//GE.render().remove(&mSprite);
	mIris->getStateFlags() |= ParticleSystem2::StateFlags::planned_deletion;
	mPupil->getStateFlags() |= ParticleSystem2::StateFlags::planned_deletion;
}

void BurningEyeRenderer::init() {
	//mSprite.getTransform().setParent(&mParent.getTransform());
	//mSprite.getDepthTransform().setParent(&mParent.getDepthTransform());

	mPupil->getTransform().setParent(&mParent.getTransform());
	mPupil->getDepthTransform().setParent(&mParent.getDepthTransform());

	mIris->getTransform().setParent(&mParent.getTransform());
	mIris->getDepthTransform().setParent(&mParent.getDepthTransform());
}
