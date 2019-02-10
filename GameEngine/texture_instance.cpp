#include "texture_instance.h"
#include "texture.h"
#include "log.h"
#include "exceptions.h"

TextureInstance::TextureInstance(Texture* in_texture) :
	mTexture(in_texture),
	mSettings(in_texture),
	mInfluences{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}

Texture* TextureInstance::getTexture() {
	return mTexture;
}

TextureSettings& TextureInstance::settings() {
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
	if (mTexture == nullptr) {
		throw InvalidArgumentException("texture was null");
	}

	mSettings.use(&mTexture->getActiveSettings(), in_slot);
}

void TextureInstance::useNone(ubyte in_slot) {
	if (mTexture == nullptr) {
		throw InvalidArgumentException("texture was null");
	}

	mTexture->useNone(in_slot);
}
