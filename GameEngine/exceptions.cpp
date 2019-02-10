#include "exceptions.h"
#include "game_engine.h"

GameEngineException::GameEngineException(const std::string& in_message) :
	mMessage(in_message)
{}

char const* GameEngineException::what() const {
	return mMessage.c_str();
}

FileNotFoundException::FileNotFoundException(const std::string& in_filename, const std::string& in_extraInfo) :
	GameEngineException(std::string("file not found: `") + in_filename + "`" + "\n [" + in_extraInfo + "]")
{}

InvalidArgumentException::InvalidArgumentException(const std::string& in_extraInfo) :
	GameEngineException(std::string("invalid argument passed") + "\n [" + in_extraInfo + "]")
{}

ProcessFailureException::ProcessFailureException(const std::string& in_extraInfo) :
	GameEngineException(std::string("process failed") + "\n [" + in_extraInfo + "]")
{}

NotImplementedException::NotImplementedException(const std::string& in_extraInfo) :
	GameEngineException(std::string("code path not implemented") + "\n [" + in_extraInfo + "]")
{}

void handleFatalException(const std::exception& in_exception) {
	std::string message = 
		std::string("a fatal error occurred. (") + 
		in_exception.what() + ") \n\n" + 
		"The application will now exit to desktop.";
	FatalAppExitA(0, message.c_str());
}

void handleFatalException() {
	std::string message =
		"An unknown fatal error occurred. \n\n"
		"The application will now exit to desktop.";
	FatalAppExitA(0, message.c_str());
}
