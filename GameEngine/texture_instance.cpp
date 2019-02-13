#include "texture_instance.h"
#include "texture.h"
#include "log.h"
#include "exceptions.h"

TextureInstance::TextureInstance(Texture* in_texture) :
	mTexture(in_texture),
	mSettings(in_texture),
	mInfluences{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}

bool TextureInstance::isNull() const {
	return mTexture == nullptr;
}

Texture* TextureInstance::getTexture() {
	return mTexture;
}

void TextureInstance::setTexture(Texture* in_texture) {
	mTexture = in_texture;
	mSettings.setTexture(in_texture);
}

TextureSettings& TextureInstance::getSettings() {
	return mSettings;
}

void TextureInstance::setInfluence(Influence in_influence, float value) {
	if (in_influence >= Influence::count) {
		throw InvalidArgumentException("invalid texture influence identifier");
	}

	mInfluences[(uint)in_influence] = value;
}

float TextureInstance::getInfluence(Influence in_influence) {
	if (in_influence >= Influence::count) {
		throw InvalidArgumentException("invalid texture influence identifier");
	}

	return mInfluences[(uint)in_influence];
}

void TextureInstance::use(ubyte in_slot) {
	if (mTexture == nullptr) return;
	mSettings.use(&mTexture->getActiveSettings(), in_slot);
}

void TextureInstance::useNone(ubyte in_slot) {
	if (mTexture == nullptr) return;
	mTexture->useNone(in_slot);
}
