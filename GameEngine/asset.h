#ifndef ASSET_H
#define ASSET_H

#include <string>

class Asset {
	friend class AssetManager;
private:
	std::string mName;

private:
	std::string Get_Name();
	void Set_Name(std::string in_name);
	void Clear_Name();

public:
	Asset();
	virtual ~Asset();
};

#endif