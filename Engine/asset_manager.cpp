#include "asset_manager.h"
#include "log.h"
#include "exceptions.h"

AssetManager::~AssetManager() {
	for (auto it = mAssets.begin(); it != mAssets.end(); it++) {
		delete it->second;
	}
	mAssets.clear();
}

void AssetManager::add(const std::string& i_name, Asset* i_asset) {
	i_asset->setName(i_name);
	std::pair<std::string, Asset*> pair(i_name, i_asset);
	if (!mAssets.insert(pair).second) {
		throw InvalidArgumentException("asset with given name already exists");
	}
}

void AssetManager::remove(Asset* i_asset) {
	remove(i_asset->getName());
}

void AssetManager::remove(const std::string & i_name) {
	mAssets.erase(i_name);
}
