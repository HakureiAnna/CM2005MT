#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
//import std.core;

#include "OrderBookEntry.h"
#include "CSVReader.h"

class OrderBook {
public:
	/**
	* construct, read in a csv data file
	*/
	OrderBook(std::string filename);
	/**
	* return vector of all known products in the dataset
	*/
	std::vector<std::string> getKnownProducts();
	/**
	* return vector of Orders according to the sent filters
	*/
	std::vector<OrderBookEntry> getOrders(
		OrderBookType type, 
		std::string product,
		std::string timestamp);

	/**
	* returns the earliest time in the orderbook
	*/
	std::string getEarliestTime();
	/**
	* returns the next time after the sent time in the orderbook
	* if there is no next timestamp, wraps around to the start
	*/
	std::string getNextTime(std::string timestamp);

	void insertOrder(OrderBookEntry& order);

	std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

	static double getHighPrice(std::vector<OrderBookEntry>& orders);
	static double getLowPrice(std::vector<OrderBookEntry>& orders);
	static double getAveragePrice(std::vector<OrderBookEntry>& orders);
	/**
	* bid-ask spread that is computed from the bids and asks from the current time slice
	* instead of the result of calling getHighPrice on bids and getLowPrice on asks since
	* that will tightly couple the two classes
	*/
	static double getSpread(
		std::vector<OrderBookEntry>& bids, 
		std::vector<OrderBookEntry>& asks);

private:
	//std::vector<OrderBookEntry> orders;
    std::map<std::string, std::map<
        OrderBookType, std::map<std::string, 
        std::vector<OrderBookEntry>>>> orders;
    
    std::set<std::string> prods;
};