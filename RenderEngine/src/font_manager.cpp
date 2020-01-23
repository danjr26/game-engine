#include "font_manager.h"
#include "log.h"

FontManager::FontManager() {
	FT_Error error = FT_Init_FreeType(&mLibrary);
	if (error) {
		throw ProcessFailureException("could not initialize font library");
	}
}

FontManager::~FontManager() {
	FT_Done_FreeType(mLibrary);
}

FT_Library FontManager::getLibrary() {
	return mLibrary;
}
