#include "asset.h"
#include "log.h"
#include "exceptions.h"

void Asset::setName(std::string in_name) {
	if (in_name == "") {
		throw InvalidArgumentException("asset name was empty string");
	}

	if (mName != "") {
		throw InvalidArgumentException("asset with given name already exists");
	}

	mName = in_name;
}

void Asset::clearName() {
	mName = "";
}

Asset::Asset() :
	mName("")
{}

Asset::~Asset()
{}

std::string Asset::getName() {
	return mName;
}
