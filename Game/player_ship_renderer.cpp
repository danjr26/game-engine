#include "player_ship_renderer.h"
#include "player_ship.h"
#include "game_engine.h"

player_ship::Renderer::Renderer(PlayerShip& i_parent) :
	mParent(i_parent),
	mSprite() {
	
	Texture* tex = GE.assets().get<Texture>("PlayerShipSpriteTexture");
	mSprite.setTexture(tex);

	Vector2d dimensions = Vector2d(Vector3d(tex->getDimensions())) / 400.0;
	AABox2d rect = AABox2d::fromCenter(Vector2d(0, 0), dimensions);
	mSprite.setRectangle(rect);

	mSprite.getTextureInstance().getSettings().setMinifyFilter(TextureSettings::FilterMode::trilinear);

	GE.render().add(&mSprite);
}

player_ship::Renderer::~Renderer() {
	if (GameEngine::exists()) {
		GE.render().remove(&mSprite);
	}
}

void player_ship::Renderer::init() {
	mSprite.getTransform().setParent(&mParent.getTransform());
	mSprite.getDepthTransform().setParent(&mParent.getDepthTransform());
}
