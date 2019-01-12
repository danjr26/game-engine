#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <unordered_map>
#include "definitions.h"
#include "asset.h"

class AssetManager {
private:
	std::unordered_map<std::string, Asset*> assets;

public:
	void Add(const std::string& in_name, Asset* in_asset);
	void Remove(Asset* in_asset);
	void Remove(const std::string& in_name);

	template<class T>
	T* Get(const std::string& in_name) {
		auto result = assets.find(in_name);
		if (result == assets.end()) {
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
	virtual void Add(const std::string& in_name, T* in_asset) {
		std::string name = (in_name == "") ?
			std::string("___") + to_string(unnamedCount++) :
			in_name;

		std::pair<std::string, T*> pair(name, in_asset);
		if (!assets.insert(pair).second) {
			Log::main("error: duplicate asset name");
			GE.Exit();
		}
	}

	virtual void Remove(const std::string& in_name) {
		assets.erase(in_name);
	}

	virtual T* Get(const std::string& in_name) {
		std::map<std::string, T*>::iterator result = assets.find(in_name);
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
