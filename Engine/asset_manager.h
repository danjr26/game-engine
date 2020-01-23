#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <unordered_map>
#include "definitions.h"
#include "asset.h"

class AssetManager {
private:
	std::unordered_map<std::string, Asset*> mAssets;

public:
	~AssetManager();

	void add(const std::string& i_name, Asset* i_asset);
	void remove(Asset* i_asset);
	void remove(const std::string& i_name);

	template<class T>
	T* get(const std::string& i_name) {
		auto result = mAssets.find(i_name);
		if (result == mAssets.end()) {
			return nullptr;
		}
		else {
			return dynamic_cast<T*>(result->second);
		}
	}
};

/*
template<class T>
class AssetManager {
private:
	static uint unnamedCount;
protected:
	std::map<std::string, T*> assets;

public:
	virtual void add(const std::string& i_name, T* i_asset) {
		std::string name = (i_name == "") ?
			std::string("___") + to_string(unnamedCount++) :
			i_name;

		std::pair<std::string, T*> pair(name, i_asset);
		if (!assets.insert(pair).second) {
			Log::main("error: duplicate asset name");
			GE.quit();
		}
	}

	virtual void remove(const std::string& i_name) {
		assets.erase(i_name);
	}

	virtual T* get(const std::string& i_name) {
		std::map<std::string, T*>::iterator result = assets.find(i_name);
		if (result == assets.end()) {
			return nullptr;
		}
		else {
			return result->second;
		}
	}
};

template<class T>
uint AssetManager<T>::unnamedCount = 1;
*/
#endif
