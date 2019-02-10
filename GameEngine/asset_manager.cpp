#include "asset_manager.h"
#include "log.h"
#include "exceptions.h"

void AssetManager::Add(const std::string& in_name, Asset* in_asset) {
	in_asset->Set_Name(in_name);
	std::pair<std::string, Asset*> pair(in_name, in_asset);
	if (!mAssets.insert(pair).second) {
		throw InvalidArgumentException("asset with given name already exists");
	}
}

void AssetManager::Remove(Asset* in_asset) {
	Remove(in_asset->Get_Name());
}

void AssetManager::Remove(const std::string & in_name) {
	mAssets.erase(in_name);
}
