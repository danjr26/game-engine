#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>
using namespace std;

typedef unsigned int uint;

class OverflowException : public exception {
private:
	string	message;
	uint	size;

public:
	OverflowException(uint in_size, string in_message);	// message format: class:function:variable
	const char* what() const throw() override final;
};

class InvalidIndexException : public exception {
private:
	string	message;
	uint	wrongIndex;
	uint	size;

public:
	InvalidIndexException(uint in_wrongIndex, uint in_size, string in_message); // message format: class:function:variable
	const char* what() const throw() override final;
};

class NullPointerException : public exception {
private:
	string message;

public:
	NullPointerException(string in_message); // message format: class:function:variable
	const char* what() const throw() override final;
};

class NotFoundException : public exception {
private:
	string message;
	string searchedFor;

public:
	NotFoundException(string in_searchedFor, string in_message); // message format: class:function:variable
	const char* what() const throw() override final;
};

class InvalidObjectException : public exception {
private:
	string message;

public:
	InvalidObjectException(string in_message);
	const char* what() const throw() override final;
};

class MismatchException : public exception {
private:
	string message;

public:
	MismatchException(string in_message);
	const char* what() const throw() override final;
};

class InvalidParameterException : public exception {
private:
	string message;

public:
	InvalidParameterException(string in_message);
	const char* what() const throw() override final;
};

class MethodFailureException : public exception {
private:
	string message;

public:
	MethodFailureException(string in_message);
	const char* what() const throw() override final;
};

#endif 
