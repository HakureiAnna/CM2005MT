#pragma once

#include <string>
#include <vector>
//import std.core;

// abstract Command class
class Command {
public:
    /**************************************
     * constructor for the command class
     * arguments:
     * _name: name of command
     * _format: required format of the command
     * _description: a short text describing purpose of command
     * _argN: no. of arguments (including the command name)
     * _errorMsg: error message to display when argument check fails
     **************************************/
	Command(std::string _name, std::string _format, std::string _description, int _argN, std::string _errorMsg);
    /**************************************
     * abstract function that implements the actual functionality of the class
     * arguments: 
     * args: the arguments to the command (including the command itself)
     * context: context specific to the current command, can be anything internal to the application required 
     **************************************/
	virtual void execute(std::vector<std::string> args, void* context) = 0;
    // getter function to get name of command    
	std::string getName();
    // getter function to get format of command
	std::string getFormat();
    // summary function to obtain a formatted string describing the command
	std::string getHelp();
    // getter function to get error message (argument validation)
    std::string getErrorMsg();
    // getter function to get no. of required arguments
    int getArgN();
    
protected:
    // name of command
	std::string name;
    // format of command
	std::string format;
    // description of command
	std::string description;
    // error message (argument validation)
    std::string errorMsg;
    // no. of arguments expected
    int argN;
};
