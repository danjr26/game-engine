#include "asset_manager.h"
#include "log.h"

void AssetManager::Add(const std::string& in_name, Asset* in_asset) {
	in_asset->Set_Name(in_name);
	std::pair<std::string, Asset*> pair(in_name, in_asset);
	if (!assets.insert(pair).second) {
		Log::main("error: duplicate asset name");
		exit(-1);
	}
}

void AssetManager::Remove(Asset* in_asset) {
	Remove(in_asset->Get_Name());
}

void AssetManager::Remove(const std::string & in_name) {
	assets.erase(in_name);
}
