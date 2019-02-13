#include "test_enemy_renderer.h"
#include "test_enemy.h"
#include "game_engine.h"

TestEnemyRenderer::TestEnemyRenderer(TestEnemy& in_parent) :
	mParent(in_parent),
	mSprite() {
	
	mSprite.setRectangle(AxisAlignedRectangled::fromCenter(Vector2d(), Vector2d(1.0, 1.0)));
	mSprite.getTransform().setParent(&mParent.getTransform());
	mSprite.getDepthTransform().setParent(&mParent.getDepthTransform());

	GE.render().add(&mSprite);
}

TestEnemyRenderer::~TestEnemyRenderer() {
	GE.render().remove(&mSprite);
}
