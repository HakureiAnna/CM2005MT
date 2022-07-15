#pragma once

#include "Command.h"

// C1: help
class HelpCommand : public Command {
public:
    // constructor for help command
	HelpCommand();
    // actual workhorse for the help command
	void execute(std::vector<std::string> args, void* context);
};

// C2: help cmd
class HelpCmdCommand : public Command {
public:
    // constructor for the help cmd command
	HelpCmdCommand();
    // actual workhorse for the help cmd command
	void execute(std::vector<std::string> args, void* context);
};

// C3: prod
class ProdCommand : public Command {
public:
    // constructor for the prod command
	ProdCommand();
    // actual workhorse for the prod command
	void execute(std::vector<std::string> args, void *context);
};

// C4: min
class MinCommand : public Command {
public:
    // constructor for the min command
	MinCommand();
    // actual workhorse for the min command
	void execute(std::vector<std::string> args, void* context);
};

// C5: max
class MaxCommand : public Command {
public:
    // constructor for the max command
	MaxCommand();
    // actual workhorse for the max command
	void execute(std::vector<std::string> args, void* context);
};

// C6: avg
class AvgCommand : public Command {
public:
    // constructor for the avg command
	AvgCommand();
    // actual workhorse for the avg command
	void execute(std::vector<std::string> args, void* context);
};

// C7: predict
class PredictCommand : public Command {
public:
    // constructor for the predict command
	PredictCommand();
    // actual workhorse for the predict command
	void execute(std::vector<std::string> args, void* context);
};

// C8: time
class TimeCommand : public Command {
public:
    // constructor for the time command
	TimeCommand();
    // actual workhorse for the time command
	void execute(std::vector<std::string> args, void* context);
};

// C9: step
class StepCommand : public Command {
public:
    // constructor for the step command
	StepCommand();
    // actual workhorse for the step command
	void execute(std::vector<std::string> args, void* context);
};
