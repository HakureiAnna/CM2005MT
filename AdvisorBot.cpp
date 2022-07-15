#include <iostream> 

#include "AdvisorBot.h"
#include "CSVReader.h"

// the main 'integrator' class that hold is essentially the entire applicationm context
/**************************************
 * constructor of AdvisorBot
 * arguments:
 * dataPath: path to the trading data CSV file
 * _predictionWindowSize: the no. of timestamps to obtain data to predict from
 * _visualizePartitions: no. of partitions to divide visualizations to
 * epsilon: the allowed floating point delta to determine if two values of equivalent 
 **************************************/
AdvisorBot::AdvisorBot(std::string dataPath, int _predictionWindowSize, int _visualizePartitions, double _epsilon):
	orderBook(dataPath), predictionWindowSize(_predictionWindowSize), visualizePartitions(_visualizePartitions), epsilon(_epsilon) {
	pastTS.push_back(orderBook.getEarliestTime());
}

/**************************************
 * destructor of AdvisorBot
 * Since AdvisorBot essentially takes over control of the dynamic memory
 * allocated to each enabled command, we need to clean up properly when done with thme
 **************************************/
AdvisorBot::~AdvisorBot() {
	for (std::map<std::string, Command*>::iterator it = commands.begin(); it != commands.end(); ++it) {
		delete it->second;
	}
}

// modifier function used to enable commands on the system
void AdvisorBot::addCommand(Command *command) {
	commands[command->getName()] = command;
}

// the main processing loop function
void AdvisorBot::run() {
	std::string input;

	std::cout << "Welcome to AdviosrBot" << std::endl;
	std::cout << "===============================================" << std::endl;
	while (true) {
		std::cout << "Enter next command: ";
		std::getline(std::cin, input);
        if (!input.size()) {
			std::cout << "Invalid command" << std::endl;
            std::cout << "===============================================" << std::endl;
            continue;
        }
		std::vector<std::string> args = CSVReader::tokenise(input, ' ');
		if (commands.count(args[0])) {
			if (args.size() > 1 && args[0] == "help") {
				args[0] = args[0] + " cmd";
			}
			commands[args[0]]->execute(args, this);
		}
		else {
			std::cout << "Invalid command" << std::endl;
		}
        std::cout << "===============================================" << std::endl;
	}
}

// getter to obtain a list of all commands available
std::map<std::string, Command*> AdvisorBot::getCommands() {
	return commands;
}

// getter for the OrderBook instance
OrderBook AdvisorBot::getOrderBook() {
	return orderBook;
}

// getter for the current timestamp
std::string AdvisorBot::getCurrentTimestamp() {
	return pastTS[pastTS.size() - 1];
}

// get a vector of past n timestamps
std::vector<std::string> AdvisorBot::getPastTimestamps(int n) {
	std::vector<std::string> retVal{};
	int TSId = pastTS.size() - 1;
	for (auto i = 0; i < n; ++i) {
		if (TSId - i < 0) {
			break;
		}
		retVal.push_back(pastTS[TSId - i]);
	}
	return retVal;
}

// used to move to the next timestamp
void AdvisorBot::updateTimestamp(std::string timestamp) {
	pastTS.push_back(timestamp);
}

// getter to obtain the prediction window size
int AdvisorBot::getPredictionWindowSize() {
	return predictionWindowSize;
}

// getter to obtain visualize partitions
int AdvisorBot::getVisualizePartitions() {
	return visualizePartitions;
}

// getter to obtain epsilon value
double AdvisorBot::getEpsilon() {
	return epsilon;
}