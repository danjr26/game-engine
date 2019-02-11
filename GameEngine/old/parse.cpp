#include "parse.h"
#include "framework.h"

void Strip_String(string& inout_string, string& in_badCharacters) {
	while (in_badCharacters.find(inout_string[0]) != string::npos) {
		inout_string = inout_string.substr(1, inout_string.length() - 1);
	}
	while (in_badCharacters.find(inout_string[inout_string.length() - 1]) != string::npos) {
		inout_string = inout_string.substr(0, inout_string.length() - 1);
	}
}

void Read_To_Char(stringstream & in_stream, string & out_string, string & in_chars) {
	uint start;
	uint end;
	char* charBuffer;

	start = in_stream.tellg();
	while(in_chars.find_first_of(in_stream.get()) == string::npos) {}
	in_stream.unget();
	end = in_stream.tellg();
	in_stream.seekg(start);

	charBuffer = new char[end - start + 1];
	in_stream.get(charBuffer, end - start);
	out_string = charBuffer;
	delete[] charBuffer;
}

void Read_To_Not_Char(stringstream & in_stream, string & out_string, string & in_chars) {
	uint start;
	uint end;
	char* charBuffer;

	start = in_stream.tellg();
	while(in_chars.find_first_of(in_stream.get()) != string::npos) {}
	in_stream.unget();
	end = in_stream.tellg();
	in_stream.seekg(start);

	charBuffer = new char[end - start + 1];
	in_stream.get(charBuffer, end - start);
	out_string = charBuffer;
	delete[] charBuffer;
}

void Ignore_To_Char(stringstream & in_stream, string & in_chars) {
	while(in_chars.find_first_of(in_stream.get()) == string::npos) {}
	in_stream.unget();
}

void Ignore_To_Not_Char(stringstream & in_stream, string & in_chars) {
	while(in_chars.find_first_of(in_stream.get()) != string::npos) {}
	in_stream.unget();
}

void Assert_Get_Char(stringstream & in_stream, string & in_chars) {
	if (in_chars.find_first_of(in_stream.get()) == string::npos) {
		throw MethodFailureException("Assert_Get_Char");
	}
}

void Assert_Get_String(stringstream & in_stream, string & in_string) {
	char* charBuffer = new char[in_string.length() + 1];
	in_stream.get(charBuffer, in_string.length());
	if (in_string != charBuffer) {
		delete[] charBuffer;
		throw MethodFailureException("Assert_Get_String");
	}
	else {
		delete[] charBuffer;
	}
}





ConfigDatum::ConfigDatum(string s) {
	int eq = s.find('=');
	key = s.substr(0, eq);
	Strip_String(key, " \t\count");

	nelements = 1;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '|')
			nelements++;
	}

	elements = new string[nelements];

	int elementi = 0;
	int prev = eq;
	for (int i = eq + 1; i < s.length(); i++) {
		if (s[i] == '|') {
			elements[elementi] = s.substr(prev + 1, i - (prev + 1));
			Strip_String(elements[elementi], " \t\count");
			elementi++;
			prev = i;
		}
	}
	elements[elementi] = s.substr(prev + 1, s.length() - (prev + 1));
	Strip_String(elements[elementi], " \t\count");
}

ConfigDatum::~ConfigDatum() {
	delete[] elements;
}

ConfigData::ConfigData(string path, string name) {
	string* lines = nullptr;
	int nlines = Get_Lines(path, name, lines, ';');
	ndata = nlines;
	for (int i = 0; i < nlines; i++) {
		if (lines[i].find('=') == string::npos)
			ndata--;
	}

	int datai = 0;
	data = new ConfigDatum*[ndata];
	for (int i = 0; i < nlines; i++) {
		if (lines[i].find('=') != string::npos) {
			data[datai] = new ConfigDatum(lines[i]);
			datai++;
		}
	}
}

ConfigData::~ConfigData() {
	for (int i = 0; i < ndata; i++)
		delete data[i];
	delete[] data;
}

ConfigDatum* ConfigData::Get(string key) {
	for (int i = 0; i < ndata; i++) {
		if (data[i]->key == key)
			return data[i];
	}
	return nullptr;
}