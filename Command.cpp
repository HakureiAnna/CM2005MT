#include "AdvisorBot.h"
#include "Command.h"
#include "OrderBook.h"

// abstract Command class
/**************************************
 * constructor for the command class
 * arguments:
 * _name: name of command
 * _format: required format of the command
 * _description: a short text describing purpose of command
 * _argN: no. of arguments (including the command name)
 * _errorMsg: error message to display when argument check fails
 **************************************/
Command::Command(std::string _name, 
				 std::string _format,
				 std::string _description,
                 int _argN,
                 std::string _errorMsg):
	name(_name),
	format(_format),
	description(_description),
    argN(_argN),
    errorMsg(_errorMsg) {}

// getter function to get name of command    
std::string Command::getName() {
	return name;
}

// getter function to get format of command
std::string Command::getFormat() {
	return format;
}

// summary function to obtain a formatted string describing the command
std::string Command::getHelp() {
	std::string helpStr = "";
	helpStr += "Command: " + name + '\n';
	helpStr += "Format: " + format + '\n';
	helpStr += "Description: " + description;
	return helpStr;
}

// getter function to get error message (argument validation)
int Command::getArgN() {
    return argN;
}

// getter function to get no. of required arguments
std::string Command::getErrorMsg() {
    return errorMsg;
}