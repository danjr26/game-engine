#include "texture_manager.h"
#include "log.h"


TextureManager::TextureManager()
{}

TextureManager::~TextureManager()
{}

void TextureManager::Add(Texture2* in_texture) {
	std::pair<std::string, Texture2*> pair(in_texture->Get_Name(), in_texture);
	if (!Texture2s.insert(pair).second) {
		Log::main("error: duplicate texture name");
		exit(-1);
	}
}

void TextureManager::Remove(const std::string& in_name) {
	Texture2s.erase(in_name);
}

Texture2* TextureManager::Get(const std::string& in_name) {
	std::map<std::string, Texture2*>::iterator result = Texture2s.find(in_name);
	if (result == Texture2s.end()) {
		return nullptr;
	}
	else {
		return result->second;
	}
}
*/
