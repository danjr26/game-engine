#include "test_enemy_renderer.h"
#include "test_enemy.h"
#include "game_engine.h"

TestEnemyRenderer::TestEnemyRenderer(TestEnemy& in_parent) :
	mParent(in_parent),
	mSprite(Circled::fromPointRadius(Vector2d(0, 0), 0.5), ColorRGBAf(0.8, 0.2, 0.2, 1.0), 2.0f) {
	
	mSprite.getTransform().setParent(&mParent.getTransform());
	mSprite.getDepthTransform().setParent(&mParent.getDepthTransform());

	GE.render().add(&mSprite);
}

TestEnemyRenderer::~TestEnemyRenderer() {
	GE.render().remove(&mSprite);
}
