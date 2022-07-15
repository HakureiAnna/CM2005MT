#pragma once

#include <string>
#include <vector>
#include <map>

#include "Command.h"
#include "OrderBook.h"

// the main 'integrator' class that hold is essentially the entire applicationm context
class AdvisorBot {
public:
    /**************************************
     * constructor of AdvisorBot
     * arguments:
     * dataPath: path to the trading data CSV file
     * _predictionWindowSize: the no. of timestamps to obtain data to predict from
     * _visualizePartitions: no. of partitions to divide visualizations to
     * epsilon: the allowed floating point delta to determine if two values of equivalent 
     **************************************/
	AdvisorBot(std::string dataPath, int _predictionWindowSize, int _visualizePartitions, double _epsilon);
    /**************************************
     * destructor of AdvisorBot
     * Since AdvisorBot essentially takes over control of the dynamic memory
     * allocated to each enabled command, we need to clean up properly when done with thme
     **************************************/
	~AdvisorBot();
    // modifier function used to enable commands on the system
	void addCommand(Command *command);
    // the main processing loop function
	void run();
    // getter to obtain a list of all commands available
	std::map<std::string, Command*> getCommands();
    // getter for the OrderBook instance
	OrderBook getOrderBook();
    // getter for the current timestamp
	std::string getCurrentTimestamp();
    // get a vector of past n timestamps
	std::vector<std::string> getPastTimestamps(int n);
    // used to move to the next timestamp
	void updateTimestamp(std::string timestamp);
    // getter to obtain the prediction window size
	int getPredictionWindowSize();
    // getter to obtain visualize partitions
	int getVisualizePartitions();
    // getter to obtain epsilon value
	double getEpsilon();

private:
    // a vector containing all previously visited timestamps
	std::vector<std::string> pastTS;;
    // a map of all enabled commands
	std::map<std::string, Command*> commands;
    // instance of OrderBook to obtain data from
	OrderBook orderBook;
    // prediction window Size (used for predict command)
	int predictionWindowSize;
    // visualize parititons (used for visualize command)
	int visualizePartitions;
    // epsilon for floating-point equivalence comparison
	double epsilon;
};