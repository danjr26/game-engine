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

	bool isNull() const;

	Texture* getTexture();
	void setTexture(Texture* in_texture);
	TextureSettings& settings();

	void setInfluence(Influence in_influence, float value);
	float getInfluence(Influence in_influence);

	void use(ubyte in_slot = 0);
	void useNone(ubyte in_slot = 0);
};

#endif