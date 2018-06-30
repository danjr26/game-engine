#include "asset.h"
#include "log.h"

void Asset::Set_Name(std::string in_name) {
	if (in_name == "") {
		Log::main("error: empty name given to asset");
		exit(-1);
	}

	if (name != "") {
		Log::main("error: cannot override extant asset name");
		exit(-1);
	}

	name = in_name;
}

void Asset::Clear_Name() {
	name = "";
}

Asset::Asset() :
	name("")
{}

Asset::~Asset()
{}

std::string Asset::Get_Name() {
	return name;
}
