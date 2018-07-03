#include "exceptions.h"

GameEngineException::GameEngineException(const std::string& in_message) :
	message(in_message)
{}

char const* GameEngineException::what() const {
	return message.c_str();
}

FileNotFoundException::FileNotFoundException(const std::string& in_filename, const std::string& in_extraInfo) :
	GameEngineException(std::string("file not found: `") + in_filename + "`" + "\n  " + in_extraInfo)
{}

InvalidArgumentException::InvalidArgumentException(const std::string& in_extraInfo) :
	GameEngineException(std::string("invalid argument passed") + "\n  " + in_extraInfo)
{}

ProcessFailureException::ProcessFailureException(const std::string& in_extraInfo) :
	GameEngineException(std::string("process failed") + "\n  " + in_extraInfo)
{}

NotImplementedException::NotImplementedException(const std::string& in_extraInfo) :
	GameEngineException(std::string("code path not implemented") + "\n  " + in_extraInfo)
{}
