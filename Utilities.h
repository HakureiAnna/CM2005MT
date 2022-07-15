#pragma once

#include <string>
#include <vector>

#include "AdvisorBot.h"
#include "Command.h"
#include "OrderBookEntry.h"

// enumeration of supported statistics in the system
enum class StatType { Unknown, Max, Min, Avg };

// a miscellaneous class used to hold utility static functions
class Utilities {
public:
    // conversion function to convert a string order book type to its OrderBookType enumeration value
    static OrderBookType stoobt(std::string s);
    // convenience cast function to cast the void* context to a usable form
    static AdvisorBot* toPAdvisorBot(void* context);
    // convenience fundamental input data validator function
    static bool checkArguments(std::vector<std::string> args, Command &cmd);
    // validator function to check if type is valid OrderBookType
    static bool validateOBT(OrderBookType type);
    // validator function to check if product is in list of available products
    static bool validateProduct(std::string product, std::vector<std::string> products);
    // validate and cast to int if possible
    static int toInt(std::string s, std::string varNme);
    // conversion function convert a string stat type to its StatType enumeration value
    static StatType stost(std::string s, bool avgValid=true);
    // validaator function to check if type is valid StatType
    static bool validateST(StatType type);
};
