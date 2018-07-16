#include "texture_instance.h"
#include "texture.h"
#include "log.h"
#include "exceptions.h"

TextureInstance::TextureInstance(Texture* in_texture) :
	texture(in_texture),
	settings(in_texture),
	influences{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}

Texture* TextureInstance::Get_Texture() {
	return texture;
}

TextureSettings& TextureInstance::Settings() {
	return settings;
}

void TextureInstance::Set_Influence(Influence in_influence, float value) {
	if (in_influence >= Influence::count) {
		throw InvalidArgumentException("invalid texture influence identifier");
	}

	influences[(uint)in_influence] = value;
}

float TextureInstance::Get_Influence(Influence in_influence) {
	if (in_influence >= Influence::count) {
		throw InvalidArgumentException("invalid texture influence identifier");
	}

	return influences[(uint)in_influence];
}

void TextureInstance::Use(ubyte in_slot) {
	if (texture == nullptr) {
		throw InvalidArgumentException("texture was null");
	}

	settings.Use(&texture->Get_Active_Settings(), in_slot);
}

void TextureInstance::Use_None(ubyte in_slot) {
	if (texture == nullptr) {
		throw InvalidArgumentException("texture was null");
	}

	texture->Use_None(in_slot);
}
