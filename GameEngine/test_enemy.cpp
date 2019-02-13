#include "test_enemy.h"
#include "game_engine.h"

TestEnemy::TestEnemy() :
	mRenderer(*this),
	mMover(*this) {
	
	GE.perFrameUpdate().add(&mMover);
}
