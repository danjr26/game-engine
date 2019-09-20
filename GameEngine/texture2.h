#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <vector>
#include <GL\glew.h>
#include "color.h"
#include "definitions.h"
#include "texture_settings.h"
#include "vector.h"
#define cimg_use_png
#include "CImg.h"
#include "texture.h"

class Texture2 : public Texture {
	friend class Framebuffer;
public:
	Texture2(std::string in_path, std::string in_name, ubyte in_nBitsPerChannel, uint in_flags = Flags::mipmaps);	
	Texture2(std::string in_name, Vector2i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel, 
		ubyte* in_data, uint in_flags = Flags::mipmaps);
	Texture2(std::string in_name, Vector2i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel, 
		ColorRGBAc in_color = ColorRGBAc(255, 255, 255, 255), uint in_flags = 0);
	~Texture2();

	const ubyte* Read();
	void Refresh_From_OpenGL();

	void Use(ubyte in_slot = 0);
	static void Use_None(ubyte in_slot = 0);

	std::string Get_Name();
	Vector2i Get_Dimensions();
	ubyte Get_Number_Channels();
	ubyte Get_Number_Bits_Per_Channel();
	uint Get_Flags();
	TextureSettings Get_Active_Settings();

protected:
	void Load_To_OpenGL();
};
#endif