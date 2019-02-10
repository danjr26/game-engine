#include "texture_instance.h"
#include "texture.h"
#include "log.h"
#include "exceptions.h"

TextureInstance::TextureInstance(Texture* in_texture) :
	mTexture(in_texture),
	mSettings(in_texture),
	mInfluences{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}

Texture* TextureInstance::Get_Texture() {
	return mTexture;
}

TextureSettings& TextureInstance::Settings() {
	return mSettings;
}

void TextureInstance::Set_Influence(Influence in_influence, float value) {
	if (in_influence >= Influence::count) {
		throw InvalidArgumentException("invalid texture influence identifier");
	}

	mInfluences[(uint)in_influence] = value;
}

float TextureInstance::Get_Influence(Influence in_influence) {
	if (in_influence >= Influence::count) {
		throw InvalidArgumentException("invalid texture influence identifier");
	}

	return mInfluences[(uint)in_influence];
}

void TextureInstance::Use(ubyte in_slot) {
	if (mTexture == nullptr) {
		throw InvalidArgumentException("texture was null");
	}

	mSettings.Use(&mTexture->Get_Active_Settings(), in_slot);
}

void TextureInstance::Use_None(ubyte in_slot) {
	if (mTexture == nullptr) {
		throw InvalidArgumentException("texture was null");
	}

	mTexture->Use_None(in_slot);
}
