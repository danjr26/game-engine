#ifndef TEXTURE2_INSTANCE_H
#define TEXTURE2_INSTANCE_H

//#include "texture2_settings.h"

class Texture2Instance {
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
	Texture2* texture;
	TextureSettings settings;
	float influences[(uint)Influence::count];

public:
	Texture2Instance(Texture2* in_texture);

	Texture2* Texture();
	TextureSettings& Settings();

	void Set_Influence(Influence in_influence, float value);
	float Get_Influence(Influence in_influence);

	void Use(ubyte in_slot = 0);
	static void Use_None(ubyte in_slot = 0);
};

#endif