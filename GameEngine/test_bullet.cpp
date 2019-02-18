#include "test_bullet.h"
#include "game_engine.h"

TestBullet::TestBullet() :
	mSprite(),
	age(0) {
	
	Texture* tex = GE.assets().get<Texture>("SparkSpriteTexture");
	mSprite.setTexture(tex);

	mSprite.setRectangle(AxisAlignedRectangled::fromCenter(Vector2d(), Vector2d(0.5, 0.5)));

	mSprite.getTransform().setParent(&getTransform());
	mSprite.getDepthTransform().setParent(&getDepthTransform());

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	GE.render().add(&mSprite);
	GE.perFrameUpdate().add(this);
}

TestBullet::~TestBullet() {
	GE.render().remove(&mSprite);
	GE.perFrameUpdate().remove(this);
}

void TestBullet::update(double in_dt) {
	age += in_dt;
	if (age >= 5.0) {
		GE.destruction().add(this);
	}

	Vector2d direction = getTransform().localToWorldDirection(Vector2d(5, 0));
	getTransform().translateWorld(direction * in_dt);
}
