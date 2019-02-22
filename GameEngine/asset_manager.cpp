#include "asset_manager.h"
#include "log.h"
#include "exceptions.h"

AssetManager::~AssetManager() {
	for (auto it = mAssets.begin(); it != mAssets.end(); it++) {
		delete it->second;
	}
	mAssets.clear();
}

void AssetManager::add(const std::string& in_name, Asset* in_asset) {
	in_asset->setName(in_name);
	std::pair<std::string, Asset*> pair(in_name, in_asset);
	if (!mAssets.insert(pair).second) {
		throw InvalidArgumentException("asset with given name already exists");
	}
}

void AssetManager::remove(Asset* in_asset) {
	remove(in_asset->getName());
}

void AssetManager::remove(const std::string & in_name) {
	mAssets.erase(in_name);
}
