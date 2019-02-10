#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "vector.h"
#include "font_face.h"
#include "asset_manager.h"
#include "render_target.h"

class FontManager {
private:
	FT_Library mLibrary;

public:
	FontManager();
	~FontManager();
	FT_Library Get_Library();
};

#endif