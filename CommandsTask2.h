#pragma once

#include "Command.h"

// custom commands
// exit command to quit the program
class ExitCommand : public Command {
public:
    // constructor of exit command
	ExitCommand();
    // actual workhorse of the exit command
	void execute(std::vector<std::string> args, void* context);
};

// spread command to compute the bid-ask spread across specified no. of timesteps
class SpreadCommand : public Command {
public:
    // constructor of spread command
	SpreadCommand();
    // actual workhorse of the spread command
	void execute(std::vector<std::string> args, void* context);
};

// visualize command to provide a very basic price plot regarding specified arguments
class VisualizeCommand : public Command {
public:
    // constructor of visualize command
	VisualizeCommand();
    // actual workhorse of the visualize command
	void execute(std::vector<std::string> args, void* context);
};

// print command to print a list of trades of specified type in the current timestep
class PrintCommand: public Command {
public:
    // constructor of print command
    PrintCommand();
    // actual workhorse of the print command
    void execute(std::vector<std::string> args, void* context);
};