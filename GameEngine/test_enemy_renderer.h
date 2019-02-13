#ifndef TEST_ENEMY_RENDERER_H
#define TEST_ENEMY_RENDERER_H

#include "sprite.h"

class TestEnemy;

class TestEnemyRenderer {
private:
	TestEnemy& mParent;
	Sprite mSprite;

public:
	TestEnemyRenderer(TestEnemy& in_parent);
	~TestEnemyRenderer();
};

#endif