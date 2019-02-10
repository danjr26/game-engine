#ifndef ASSET_H
#define ASSET_H

#include <string>

class Asset {
	friend class AssetManager;
private:
	std::string mName;

private:
	std::string getName();
	void setName(std::string in_name);
	void clearName();

public:
	Asset();
	virtual ~Asset();
};

#endif