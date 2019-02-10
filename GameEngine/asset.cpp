#include "asset.h"
#include "log.h"
#include "exceptions.h"

void Asset::Set_Name(std::string in_name) {
	if (in_name == "") {
		throw InvalidArgumentException("asset name was empty string");
	}

	if (mName != "") {
		throw InvalidArgumentException("asset with given name already exists");
	}

	mName = in_name;
}

void Asset::Clear_Name() {
	mName = "";
}

Asset::Asset() :
	mName("")
{}

Asset::~Asset()
{}

std::string Asset::Get_Name() {
	return mName;
}
