#include "burning_eye_renderer.h"
#include "burning_eye.h"
#include "burning_eye_iris_specifier.h"
#include "burning_eye_pupil_specifier.h"
#include "game_engine.h"
#include "particle_system2_factories.h"

burning_eye::Renderer::Renderer(BurningEye& in_parent) :
	mParent(in_parent),
	mSprite(Circled::fromPointRadius(Vector2d(0, 0), 0.5), ColorRGBAf(0.8, 0.2, 0.2, 1.0), 2.0f),
	mIris(nullptr),
	mPupil(nullptr) {

	mPupil = createSimpleParticleSystem2<PupilSpecifier>(Vector3d(), "GlowTexture");
	mPupil->setBlendSettings(BlendSettings::subtractive());
	mPupil->getDepthTransform().setLocalDepth(0.01);

	mIris = createSimpleParticleSystem2<IrisSpecifier>(Vector3d(), "FlameTexture");
	mIris->setBlendSettings(BlendSettings::additive());

	//GE.render().add(&mSprite);
}

burning_eye::Renderer::~Renderer() {
	//GE.render().remove(&mSprite);
	mIris->getStateFlags() |= ParticleSystem2::StateFlags::planned_deletion;
	mPupil->getStateFlags() |= ParticleSystem2::StateFlags::planned_deletion;
}

void burning_eye::Renderer::init() {
	//mSprite.getTransform().setParent(&mParent.getTransform());
	//mSprite.getDepthTransform().setParent(&mParent.getDepthTransform());

	mPupil->getTransform().setParent(&mParent.getTransform());
	mPupil->getDepthTransform().setParent(&mParent.getDepthTransform());

	mIris->getTransform().setParent(&mParent.getTransform());
	mIris->getDepthTransform().setParent(&mParent.getDepthTransform());
}
