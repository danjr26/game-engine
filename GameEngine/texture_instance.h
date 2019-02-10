#ifndef TEXTURE_INSTANCE_H
#define TEXTURE_INSTANCE_H

#include "texture_settings.h"

class Texture;

class TextureInstance {
public:
	enum class Influence : uint {
		ambient,
		diffuse,
		specular,
		hardness,
		emission,
		intensity,
		alpha,
		normal,
		count
	};
private:
	Texture* mTexture;
	TextureSettings mSettings;
	float mInfluences[(uint)Influence::count];

public:
	TextureInstance(Texture* in_texture);

	Texture* Get_Texture();
	TextureSettings& Settings();

	void Set_Influence(Influence in_influence, float value);
	float Get_Influence(Influence in_influence);

	void Use(ubyte in_slot = 0);
	void Use_None(ubyte in_slot = 0);
};

#endif