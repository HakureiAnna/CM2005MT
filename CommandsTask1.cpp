#include <iostream>

#include "CommandsTask1.h"
#include "Utilities.h"

// C1: help
// constructor for help command
HelpCommand::HelpCommand() : Command(
	"help",
	"help", 
	"List all available commands.",
    1,
    "The help command takes no arguments."
) {}
    
// actual workhorse for the help command
void HelpCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
    // get commands and output their name
	auto commands = Utilities::toPAdvisorBot(context)->getCommands();
	for (auto itor = commands.begin(); itor != commands.end(); ++itor) {
		std::cout << itor->second->getName() << std::endl;
	}
}

// C2: help cmd
// constructor for the help cmd command
HelpCmdCommand::HelpCmdCommand() : Command(
	"help cmd", 
	"help cmd", 
	"Output detailed help for specified command.",
    2,
    "The help command requires a 'command' argument specified to obtain help content regarding the command."
) {}

// actual workhorse for the help cmd command
void HelpCmdCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
    std::string cmd = args[1];
    
    // get commands and output the help documentation for specified command
	auto commands = Utilities::toPAdvisorBot(context)->getCommands();
    if (commands.count(cmd) < 1) {
        std::cout << cmd << " is not an available command." << std::endl;
        return;
    }
	std::cout << commands[cmd]->getHelp() << std::endl;
}

// C3: prod
// constructor for the prod command
ProdCommand::ProdCommand() : Command(
	"prod", 
	"prod", 
	"List all available products.",
    1,
    "The prod command takes no arguments"
) {}

// actual workhorse for the prod command
void ProdCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
    // get list of all products known and print them
	auto products = Utilities::toPAdvisorBot(context)->getOrderBook().getKnownProducts();
	for (auto itor = products.begin(); itor != products.end(); ++itor) {
		std::cout << *itor << std::endl;
	}
}

// C4: min
// constructor for the min command
MinCommand::MinCommand() : Command(
	"min", 
	"min product bid|ask", 
	"Find the minimum bid/ ask of the specified product in the current timestep.",
    3,
    "The min command requires product, bid|ask as arguments to function."
) {}

// actual workhorse for the min command
void MinCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
	auto c = Utilities::toPAdvisorBot(context);
	auto orderBook = c->getOrderBook();
    std::string orderType = args[2];
	OrderBookType type {Utilities::stoobt(orderType)};
    if (!Utilities::validateOBT(type)) {
        return;
    }
    std::string product = args[1];
    if (!Utilities::validateProduct(product, orderBook.getKnownProducts())) {
        return;
    }    
	auto orders = orderBook.getOrders(type, product, c->getCurrentTimestamp());
    
    // compute and output the min price
	auto minPrice = orderBook.getLowPrice(orders);
	std::cout << "The minimum " << orderType << " for " << product << " is " << minPrice << std::endl;
}

// C5: max
// constructor for the max command
MaxCommand::MaxCommand() : Command(
	"max",
	"max product bid|ask",
	"Find the maximum bid/ ask of the specified product in the current timestep.",
    3,
    "The max command requires product, bid|ask to function."
) {}

// actual workhorse for the max command
void MaxCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
	auto c = Utilities::toPAdvisorBot(context);
	auto orderBook = c->getOrderBook();
    std::string orderType = args[2];
	OrderBookType type{Utilities::stoobt(orderType)};
    if (!Utilities::validateOBT(type)) {
        return;
    }
    std::string product = args[1];
    if (!Utilities::validateProduct(product, orderBook.getKnownProducts())) {
        return;
    }
	auto orders = orderBook.getOrders(type, product, c->getCurrentTimestamp());
    
    // compute and output the max price
	auto maxPrice = orderBook.getHighPrice(orders);
	std::cout << "The maximum " << orderType << " for " << product << " is " << maxPrice << std::endl;
}

// C6: avg
// constructor for the avg command
AvgCommand::AvgCommand() : Command(
	"avg",
	"avg product ask|bid timesteps",
	"Compute the average ask/ bid of the specified product over the specified number of timesteps.",
    4,
    "The avg command requires product, ask|bid, timesteps arguments to function."
) {}

// actual workhorse for the avg command
void AvgCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
	auto n = Utilities::toInt(args[3], "n");
    if (n < 0) {
        return;
    }
	auto c = Utilities::toPAdvisorBot(context);
	auto orderBook = c->getOrderBook();
	auto timestamps = c->getPastTimestamps(n);
    
    // compute and output the average price
	int count = 0;
	double total = 0;
    std::string orderType = args[2];
	OrderBookType type{Utilities::stoobt(orderType)};
    if (!Utilities::validateOBT(type)) {
        return;
    }
    std::string product = args[1];
    if (!Utilities::validateProduct(product, orderBook.getKnownProducts())) {
        return;
    }
    // iterate over all trading data in specified period and compute the total and count
	for (auto itor = timestamps.begin(); itor != timestamps.end(); ++itor) {
		auto orders = orderBook.getOrders(type, product, *itor);
		for (auto itor2 = orders.begin(); itor2 != orders.end(); ++itor2) {
			total += itor2->price;
			count++;
		}
	}
	std::cout << "average " << orderType << " price of " << product << " across " << n << " timestamps is " << (total / count) << std::endl;
}

// C7: predict
// constructor for the predict command
PredictCommand::PredictCommand() : Command(
	"predict",
	"predict max|min product ask|bid",
	"Predict the max/ min ask/ bid price of the specified product for the next timestep.",
    4,
    "The predict command requires max|min, product, ask|bid arguments to function."
) {}

// actual workhorse for the predict command
void PredictCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
	auto c = Utilities::toPAdvisorBot(context);
	auto orderBook = c->getOrderBook();
    
    std::string orderType = args[3];
	OrderBookType type{Utilities::stoobt(orderType)};
    if (!Utilities::validateOBT(type)) {
        return;
    }
    std::string product = args[2];
    if (!Utilities::validateProduct(product, orderBook.getKnownProducts())) {
        return;
    }
	int n = c->getPredictionWindowSize();
    std::string stat = args[1];
    StatType statType = Utilities::stost(stat, false);
    if (!Utilities::validateST(statType)) {
        return;
    }
        
    // predict the next max/ min ask/bid using historic data
	auto timestamps = c->getPastTimestamps(n);
	double total = 0;
	int count = 0;
    // iterate over trading data in the specified period and compute total and count 
	for (auto itor = timestamps.begin(); itor != timestamps.end(); ++itor) {
		auto orders = orderBook.getOrders(type, product, *itor);
        switch (statType) {
        case StatType::Max:
			total += orderBook.getHighPrice(orders);
            break;
        case StatType::Min:
			total += orderBook.getLowPrice(orders);
            break;                
        }
		count++;
	}

	std::cout << "The predicted " << stat << " price for " << orderType << " of " << product << " is " << (total / count) << std::endl;
}

// C8: time
// constructor for the time command
TimeCommand::TimeCommand() : Command(
	"time",
	"time",
	"Print the current timestep.",
    1,
    "The time command takes no arguments."
) {}

// actual workhorse for the time command
void TimeCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
    // obtain and print the current time step
	auto ts = Utilities::toPAdvisorBot(context)->getCurrentTimestamp();
	std::cout << ts << std::endl;
}

// C9: step
// constructor for the step command
StepCommand::StepCommand() : Command(
	"step",
	"step",
	"Move to and print the next timestep.",
    1,
    "The step command takes no arguments."
) {}

// actual workhorse for the step command
void StepCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
    // go to the next time stamp and print 
	auto c = Utilities::toPAdvisorBot(context);
	c->updateTimestamp(c->getOrderBook().getNextTime(c->getCurrentTimestamp()));
    std::cout << "we have advanced to " << c->getCurrentTimestamp() << std::endl;
}
