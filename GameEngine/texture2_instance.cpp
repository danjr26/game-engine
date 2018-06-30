#include "texture2_instance.h"
#include "texture2.h"
#include "log.h"

Texture2Instance::Texture2Instance(Texture2* in_texture):
	texture(in_texture),
	settings(in_texture),
	influences{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f} 
{}

Texture2* Texture2Instance::Texture() {
	return texture;
}

TextureSettings& Texture2Instance::Settings() {
	return settings;
}

void Texture2Instance::Set_Influence(Influence in_influence, float value) {
	if (in_influence >= Influence::count) {
		Log::main("error: invalid texture influence identifier");
		exit(-1);
	}

	influences[(uint)in_influence] = value;
}

float Texture2Instance::Get_Influence(Influence in_influence) {
	if (in_influence >= Influence::count) {
		Log::main("error: invalid texture influence identifier");
		exit(-1);
	}

	return influences[(uint)in_influence];
}

void Texture2Instance::Use(ubyte in_slot) {
	if (texture == nullptr) {
		Log::main("error: null texture");
		exit(-1);
	}

	settings.Use(&texture->Get_Active_Settings(), in_slot);
}

void Texture2Instance::Use_None(ubyte in_slot) {
	Texture2::Use_None(in_slot);
}
