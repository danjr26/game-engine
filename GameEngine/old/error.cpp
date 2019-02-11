#include "error.h"

OverflowException::OverflowException(uint in_size, string in_message) :
message	(in_message),
size	(in_size)
{}

const char * OverflowException::what() const throw() {
	return (string("Error: data overflow in container of size " + to_string(size) + ".\n") + message).c_str();
}

InvalidIndexException::InvalidIndexException(uint in_wrongIndex, uint in_size, string in_message) :
message		(in_message),
wrongIndex	(in_wrongIndex),
size		(in_size)
{}

const char * InvalidIndexException::what() const throw() {
	return (string("Error: invalid index " + to_string(wrongIndex) + " in container of size " + to_string(size) + ".\n" + message)).c_str();
}

NullPointerException::NullPointerException(string in_message) :
message(in_message)
{}

const char * NullPointerException::what() const throw() {
	return (string("Error: null pointer.\n") + message).c_str();
}

NotFoundException::NotFoundException(string in_searchedFor, string in_message) :
message		(in_message),
searchedFor	(in_searchedFor)
{}

const char * NotFoundException::what() const throw() {
	return (string("Error: <") + searchedFor + string("> not found.\n") + message).c_str();
}

InvalidObjectException::InvalidObjectException(string in_message) :
message(in_message)
{}

const char * InvalidObjectException::what() const throw() {
	return (string("Error: invalid object accessed.\n") + message).c_str();
}

MismatchException::MismatchException(string in_message) :
message(in_message)
{}

const char * MismatchException::what() const throw() {
	return ("Error: sanity check failed.\n" + message).c_str();
}

InvalidParameterException::InvalidParameterException(string in_message) :
message(in_message)
{}

const char * InvalidParameterException::what() const throw() {
	return ("Error: invalid parameter.\n" + message).c_str();
}

MethodFailureException::MethodFailureException(string in_message) :
message(in_message)
{}

const char * MethodFailureException::what() const throw() {
	return ("Error: method failure.\n" + message).c_str();
}
