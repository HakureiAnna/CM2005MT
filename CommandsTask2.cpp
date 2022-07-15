#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>

#include "CommandsTask2.h"
#include "Utilities.h"

// custom
// exit command to quit the program
    // constructor of exit command
ExitCommand::ExitCommand() : Command(
	"exit",
	"exit",
	"Quit the AdvisorBot application.",
    1,
    "The exit command takes no arguments."
) {}

// actual workhorse of the exit command
void ExitCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
    // terminate application
	exit(0);
}

// actual workhorse of the exit command
    // constructor of spread command
SpreadCommand::SpreadCommand() : Command(
	"spread", 
	"spread product timesteps",
	"Compute the bid-ask spread of the specified product across the specified number of timesteps.",
    3,
    "The spread command requires product, timesteps as arguments to function."
)
{}

// actual workhorse of the spread command
void SpreadCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
	auto c = Utilities::toPAdvisorBot(context);
	auto orderBook = c->getOrderBook();
	auto n = Utilities::toInt(args[2], "n");
    if (n < 0) {
        return;
    }
    std::string product = args[1];
    if (!Utilities::validateProduct(product, orderBook.getKnownProducts())) {
        return;
    }
    
	auto pastTS = c->getPastTimestamps(n);
	std::vector<OrderBookEntry> asks{};
	std::vector<OrderBookEntry> bids{};
	for (auto itor = pastTS.begin(); itor != pastTS.end(); ++itor) {
		auto currentAsks = orderBook.getOrders(
			OrderBookType::ask,
			product,
			*itor
		);
		auto currentBids = orderBook.getOrders(
			OrderBookType::bid,
			product,
			*itor
		);
		asks.insert(asks.end(), currentAsks.begin(), currentAsks.end());
		bids.insert(bids.end(), currentBids.begin(), currentBids.end());
	}
	auto spread = orderBook.getSpread(bids, asks);
	std::cout << "The Bid-Ask Spread for " << product << " across the past " << n << " timesteps is " << spread << std::endl;
}

// visualize command to provide a very basic price plot regarding specified arguments
    // constructor of visualize command
VisualizeCommand::VisualizeCommand() : Command(
	"visualize",
	"visualize product bid|ask min|max|avg timesteps",
	"Plot a fundamental visualization of the min/ max/ avg ask/bid price of the specified product across the specified number of timesteps.",
    5,
    "The visualize command requires product, bid|ask, min|max|avg, timesteps as arguments to function."
) {}

// actual workhorse of the visualize command
void VisualizeCommand::execute(std::vector<std::string> args, void* context) {
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
    std::string stat = args[3];
    StatType statType = Utilities::stost(stat);
    if (!Utilities::validateST(statType)) {
        return;
    }
	auto n = Utilities::toInt(args[4], "n");
    if (n < 0) {
        return;
    }
	auto pastTS = c->getPastTimestamps(n);
    // validate at least 2 past timestamps worth of data is obtainable
    if (pastTS.size() < 2) {
        std::cout << "A minimum of 2 timesteps of data is required to visualize the data." << std::endl;
    }
    
    // compute plotting required values
	reverse(pastTS.begin(), pastTS.end());
	std::vector<double> values;
	double minValue = std::numeric_limits<double>::max();
	double maxValue = std::numeric_limits<double>::min();
    // compute the min and max price of the trading data to be plotted
	for (auto itor = pastTS.begin(); itor != pastTS.end(); ++itor) {
		auto orders = orderBook.getOrders(type, args[1], *itor);
		double value = 0;
		switch (statType) {
        case StatType::Min:
			value = OrderBook::getLowPrice(orders);
			break;
        case StatType::Max:
			value = OrderBook::getHighPrice(orders);
			break;
        case StatType::Avg:
			value = OrderBook::getAveragePrice(orders);
			break;
		}
		if (value < minValue) {
			minValue = value;
		} 
		if (value > maxValue) {
			maxValue = value;
		}
		values.push_back(value);
	}

    // normalize the price into the partition ranges
	std::vector<double> range{};
	double r = maxValue - minValue;
	auto m = c->getVisualizePartitions();
    // first, compute each partitions range
	for (int i = 0; i <= m; ++i) {
		double value = minValue + r * i / n;
		range.push_back(value);
	}

	double epsilon = c->getEpsilon();
	std::vector<int> valuesP;
    // next, use floating-point comparison, to fit the price data into each partitions range
	for (auto itor = values.begin(); itor != values.end(); ++itor) {
		int x= 0;
		for (auto i = 0; i < range.size(); ++i) {
			if (std::abs(*itor - range[i]) < epsilon) {
				x = i;
				break;
			}
		}
		valuesP.push_back(x);
	}

    // create the formatted strings required for the final output
    char buffer[255];
    // resorting to old school C formatting since we are confining ourselves
    // to the limitations of C++11, and the std::format() utility function is
    // only made available in the C++20 standard, for which compiler support 
    // is at a minimum (during testing only vc++ of VS2022 has support for modern C++)
    sprintf(buffer, "%16s |", " ");
	std::string bottom = buffer;
	std::string top = "----";
	std::string side = "|  |";
	std::string space = "    ";
    // create bottom 'axis' that shows each timestamp
	for (auto i=0; i<pastTS.size(); ++i) {
        sprintf(buffer, "[%2d]|", i + 1);
		bottom += buffer;
	}
	std::vector<std::string> lines{};
	lines.push_back(bottom);
	std::cout << "Values count: " << valuesP.size() << std::endl;
    // create the actual plot line by line horizontally, iterating over partitions in the outer loop
	for (auto i = 0; i <= m; ++i) {
        sprintf(buffer, "%16.7lf ", range[i]);
		std::string line = buffer;
        // iterating over each timestamp's price data in the inner loop
		for (auto itor = valuesP.begin(); itor != valuesP.end(); ++itor) {
			if (*itor < i) {
				line += " " + space;
			} else if (*itor == i) {
				line += " " + top;
			} else {
				line += " " + side;
			}
		}
		lines.push_back(line);
	}
    
    // perform actual drawing of the plot data
	reverse(lines.begin(), lines.end());
	for (auto itor = lines.begin(); itor != lines.end(); ++itor) {
		std::cout << *itor << std::endl;
	}
	for (auto i = 0; i <  pastTS.size(); ++i) {
        sprintf(buffer, "[%2d]: %s", i + 1, pastTS[i].c_str());
		std::cout << buffer << std::endl;
	}
}

// print command to print a list of trades of specified type in the current timestep
// constructor of print command
PrintCommand::PrintCommand(): Command(
    "print",
    "print ask|bid product",
    "Print a list of all the asks/ bids of the specified product within the current timestep.",
    3,
    "The print command requires ask|bid, product arguments to function."
) {}

// actual workhorse of the print command
void PrintCommand::execute(std::vector<std::string> args, void* context) {
    // input validation and initialization
    if (!Utilities::checkArguments(args, *this)) {
        return;
    }
	auto c = Utilities::toPAdvisorBot(context);
    auto orderBook = c->getOrderBook();
    std::string orderType = args[1];
    auto type {Utilities::stoobt(orderType)};
    if (!Utilities::validateOBT(type)) {
        return;
    }
    auto ts = c->getCurrentTimestamp();
    std::string product = args[2];
    if (!Utilities::validateProduct(product, orderBook.getKnownProducts())) {
        return;
    }
    auto orders = orderBook.getOrders(type, product, ts);
    
    // print he ader information
    std::cout << "Current Timestep: " << ts << std::endl;
    std::cout << product << " " << orderType << "s" << std::endl;
    
    // print formatted table of the price and amount of each trade related to the product specified in the header
    char buffer[255];
    sprintf(buffer, "| %3s | %10s | %10s |", "No.", "price", "amount");
    std::string sep = "";
    // table header
    for (int i=0; i<strlen(buffer); ++i) {
        sep += "-";
    }
    std::cout << sep << std::endl;
    std::cout << buffer << std::endl;
    std::cout << sep << std::endl;
    int i = 1;
    // actual table row iteration
    for (auto itor=orders.begin(); itor != orders.end(); ++itor) {
        sprintf(buffer, "| %3d | %10.5lf | %10.5lf |", i++, itor->price, itor->amount);
        std::cout << buffer << std::endl;
    }
    std::cout << sep << std::endl;
}