#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class GameEngineException : public std::exception {
	std::string message;

public:
	GameEngineException(const std::string& in_message);

	char const* what() const override;
};

class FileNotFoundException : public GameEngineException {
public:
	FileNotFoundException(const std::string& in_filename, const std::string& in_extraInfo = std::string("no further information given"));
};

class InvalidArgumentException : public GameEngineException {
public:
	InvalidArgumentException(const std::string& in_extraInfo = std::string("no further information given"));
};

class ProcessFailureException : public GameEngineException {
public:
	ProcessFailureException(const std::string& in_extraInfo = std::string("no further information given"));
};

class NotImplementedException : public GameEngineException {
public:
	NotImplementedException(const std::string& in_extraInfo = std::string("no further information given"));
};


void Handle_Fatal_Exception(const std::exception& in_exception);
void Handle_Fatal_Exception();

#endif