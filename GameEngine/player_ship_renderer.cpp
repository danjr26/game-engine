#include "player_ship_renderer.h"
#include "player_ship.h"
#include "game_engine.h"

PlayerShipRenderer::PlayerShipRenderer(PlayerShip& in_parent) :
	mParent(in_parent),
	mSprite() {
	
	Texture* tex = GE.assets().get<Texture>("PlayerShipSpriteTexture");
	mSprite.setTexture(tex);

	Vector2d dimensions = Vector2d(Vector3d(tex->getDimensions())) / 400.0;
	AxisAlignedRectangled rect = AxisAlignedRectangled::fromCenter(Vector2d(0, 0), dimensions);
	mSprite.setRectangle(rect);

	mSprite.getTransform().setParent(&mParent.getTransform());
	mSprite.getDepthTransform().setParent(&mParent.getDepthTransform());

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	GE.render().add(&mSprite);
}

PlayerShipRenderer::~PlayerShipRenderer() {
	GE.render().remove(&mSprite);
}
