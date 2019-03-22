#ifndef TEST_ENEMY_RENDERER_H
#define TEST_ENEMY_RENDERER_H

#include "circle_renderer.h"

class TestEnemy;

class TestEnemyRenderer {
private:
	TestEnemy& mParent;
	CircleRenderer mSprite;

public:
	TestEnemyRenderer(TestEnemy& in_parent);
	~TestEnemyRenderer();

	void init();
};

#endif