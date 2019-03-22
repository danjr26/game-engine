#ifndef TEST_ENEMY_HEALTH_H
#define TEST_ENEMY_HEALTH_H

class TestEnemy;

class TestEnemyHealth {
private:
	TestEnemy& mParent;

public:
	TestEnemyHealth(TestEnemy& in_parent);
};

#endif