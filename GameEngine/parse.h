#ifndef PARSE_H
#define PARSE_H

#include <istream>
#include <string>
using namespace std;

void Strip_String(string& inout_string, string& in_badCharacters);
void Read_To_Char(stringstream& in_stream, string& out_string, string& in_chars);
void Read_To_Not_Char(stringstream& in_stream, string& out_string, string& in_chars);
void Ignore_To_Char(stringstream& in_stream, string& in_chars);
void Ignore_To_Not_Char(stringstream& in_stream, string& in_chars);
void Assert_Get_Char(stringstream& in_stream, string& in_chars);
void Assert_Get_String(stringstream& in_stream, string& in_string);

class ConfigDatum {
public:
	string	key;
	string* elements;
	int		nelements;

	ConfigDatum	(string s);
	~ConfigDatum();
};

class ConfigData {
public:
	ConfigDatum**	data;
	int				ndata;

	ConfigData		(string path, string name);
	~ConfigData		();
	ConfigDatum* Get(string key);
};

#endif