#include <algorithm>
#include <iostream>

#include "Utilities.h"

// a miscellaneous class used to hold utility static functions
// conversion function to convert a string order book type to its OrderBookType enumeration value
OrderBookType Utilities::stoobt(std::string s) {
	OrderBookType type{};
	if (s == "bid") {
		type = OrderBookType::bid;
	}
	else if (s == "ask") {
		type = OrderBookType::ask;
	}
    return type;
}

// convenience cast function to cast the void* context to a usable form
AdvisorBot* Utilities::toPAdvisorBot(void* context) {
    return static_cast<AdvisorBot*>(context);
}

// convenience fundamental input data validator function
bool Utilities::checkArguments(std::vector<std::string> args, Command &cmd) {
    if (args.size() != cmd.getArgN()) {
        std::cout << cmd.getErrorMsg() << std::endl;
        return false;
    }
    return true;
}

// validator function to check if type is valid OrderBookType
bool Utilities::validateOBT(OrderBookType type) {
    if (type == OrderBookType::unknown) {
        std::cout << "The specified order type is invalid." << std::endl;
        return false;
    }
    return true;
}

// validator function to check if product is in list of available products
bool Utilities::validateProduct(std::string product, std::vector<std::string> products) {
    if (std::count(products.begin(), products.end(), product) < 1) {
        std::cout << "Product of type " << product << " is unavailable." << std::endl;
        return false;
    }
    return true;
}

// validate and cast to int if possible
int Utilities::toInt(std::string s, std::string varName) {
    int n = 0;
    try {
        n = stoi(s);
    } catch (std::exception& e) {
        std::cout << varName << " requires an integer value." << std::endl;
        return -1;
    }
    return n;
}

// conversion function convert a string stat type to its StatType enumeration value
StatType Utilities::stost(std::string s, bool avgValid) {
    StatType type = StatType::Unknown;
    if (s == "max") {
        type = StatType::Max;
    } else if (s == "min") {
        type = StatType::Min;
    } else if (avgValid && s == "avg") {
        type = StatType::Avg;
    }
    return type;
}

// validaator function to check if type is valid StatType
bool Utilities::validateST(StatType type) {
    if (type == StatType::Unknown) {
        std::cout << "The specified stat type is invalid." << std::endl;
        return false;
    }
    return true;
}