#include "font_manager.h"
#include "log.h"

FontManager::FontManager() {
	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		Log::main("error: failure to initialize font library");
	}
}

FontManager::~FontManager() {
	FT_Done_FreeType(library);
}

FT_Library FontManager::Get_Library() {
	return library;
}
