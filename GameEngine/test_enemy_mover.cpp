#include "test_enemy_mover.h"
#include "game_engine.h"
#include "test_enemy.h"

TestEnemyMover::TestEnemyMover(TestEnemy& in_parent) :
	mParent(in_parent)
{}

void TestEnemyMover::update(double in_dt) {
	PlayerShip* target = GE.game().getPlayerShip();
	Vector2d velocity = target->getTransform().getWorldPosition() - mParent.getTransform().getWorldPosition();
	velocity = velocity.normalized() * 1.0;

	mParent.getTransform().translateLocal(velocity * in_dt);
}
