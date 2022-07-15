#include <map>
#include <algorithm>
//import std.core;

#include "OrderBook.h"
/**
* construct, read in a csv data file
*/
OrderBook::OrderBook(std::string filename) {
    //orders = CSVReader::readCSV(filename);
	std::vector<OrderBookEntry> _orders = CSVReader::readCSV(filename);
    
    for (int i=0; i<_orders.size(); ++i) {
        insertOrder(_orders[i]);
    }
}

/**
* return vector of all known products in the dataset
*/
std::vector<std::string> OrderBook::getKnownProducts() {
	//for (OrderBookEntry& e : orders) {
	//	prodMap[e.product] = true;
	//}

	// now flatten the map to a vector of strings
	//for (auto const& e : prodMap) {
	//	products.push_back(e.first);
	//}

	return std::vector<std::string>(prods.begin(), prods.end());
}

/**
* return vector of Orders according to the sent filters
*/
std::vector<OrderBookEntry> OrderBook::getOrders(
	OrderBookType type,
	std::string product,
	std::string timestamp) {
	std::vector<OrderBookEntry> orders_sub;
	
    return orders[timestamp][type][product];
	//for (OrderBookEntry& e : orders) {
	//	if (e.orderType == type &&
	//		e.product == product &&
	//		e.timestamp == timestamp
	//		) {
	//		orders_sub.push_back(e);
	//	}
	//}

	//return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders) {
	double max = orders[0].price;
	for (OrderBookEntry& e : orders) {
		if (e.price > max) {
			max = e.price;
		}
	}
	return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders) {
	double min = orders[0].price;
	for (OrderBookEntry& e : orders) {
		if (e.price < min) {
			min = e.price;
		}
	}
	return min;
}

double OrderBook::getAveragePrice(std::vector<OrderBookEntry>& orders) {
	double total = 0;
	int count = 0;
	for (auto itor = orders.begin(); itor != orders.end(); ++itor) {
		total += itor->price;
		count++;
	}
	count = count > 0 ? count : 1;
	return total / count;
}


/**
* returns the earliest time in the orderbook
*/
std::string OrderBook::getEarliestTime() {
	//return orders[0].timestamp;
	//auto ts = orders[0].timestamp;
	//for (OrderBookEntry& e : orders) {
	//	if (e.timestamp < ts) {
	//		ts = e.timestamp;
	//	}
	//}
	//return ts;
    return orders.begin()->first;
}

/**
	* returns the next time after the sent time in the orderbook
	* if there is no next timestamp, wraps around to the start
*/
std::string OrderBook::getNextTime(std::string timestamp) {
	std::string next_timestamp = "";
	//for (OrderBookEntry& e : orders) {
	//	if (e.timestamp > timestamp) {
	//		next_timestamp = e.timestamp;
	//		break;
	//	}
	//}
	//if (next_timestamp == "") {
	//	next_timestamp = orders[0].timestamp;
	//}
	//return next_timestamp;
    for (auto itor=orders.begin(); itor != orders.end(); ++itor) {
        if (itor->first == timestamp) {
            ++itor;
            return itor->first;
        }
    }
    return orders.begin()->first;
}

/**
* bid-ask spread that is computed from the bids and asks from the current time slice
* instead of the result of calling getHighPrice on bids and getLowPrice on asks since
* that will tightly couple the two classes
*/
double OrderBook::getSpread(
	std::vector<OrderBookEntry>& bids,
	std::vector<OrderBookEntry>& asks) {
	double highBid = getHighPrice(bids);
	double lowAsk = getLowPrice(asks);
	
	return highBid - lowAsk;
}

void OrderBook::insertOrder(OrderBookEntry& order) {    
    orders[order.timestamp][order.orderType][order.product].push_back(order);
    prods.insert(order.product);
    //orders.push_back(order);
	//d::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}


std::vector<OrderBookEntry> OrderBook::matchAsksToBids(
	std::string product, 
	std::string timestamp) {
	//asks = orderbook.asks
	std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
	//bids = orderbook.bids
	std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);

	//sales = []
	std::vector<OrderBookEntry> sales;
	//sort asks lowest first
	std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
	//sort bids highest first
	std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

	//for ask in asks:
	for (OrderBookEntry& ask : asks) {
		//	for bid in bids:
		for (OrderBookEntry& bid : bids) {
			//		if bid.price >= ask.price # we have a match
			if (bid.price >= ask.price) {
				//			sale = new order()
				OrderBookEntry sale{
					//			sale.price = ask.price
					ask.price,
					0,
					timestamp,
					product,
					OrderBookType::asksale
				};

				if (bid.username == "simuser") {
					sale.username = "simuser";
					sale.orderType = OrderBookType::bidsale;
				}
				if (ask.username == "simuser") {
					sale.username = "simuser";
					sale.orderType = OrderBookType::asksale;
				}

				//			# now work out how much was sold and
				//			# create new bids and asks covering
				//			# anything that aws not sold
				//			if bid.amount == ask.amount: # bid completely clears ask
				if (bid.amount == ask.amount) {
					//				sale.amount = ask.amount
					sale.amount = ask.amount;
					//				sales.append(sale)
					sales.push_back(sale);
					//				bid.amount = 0 # make sure the bid is not processed again
					bid.amount = 0;
					//				# can do no more with this ask
					//				# go onto the next ask
					//				break
					break;
				}

				//			if bid.amount > ask.amount: # ask is completely gone slice the bid amount
				if (bid.amount > ask.amount && bid.amount > 0) {
					//				sale.amount = ask.amount
					sale.amount = ask.amount;
					//				sales.append(sale)
					sales.push_back(sale);
					//				# we adjust the bid in place
					//				# so it can be used to process the next ask
					//				bid.amount = bid.amount - ask.amount
					bid.amount -= ask.amount;
					//				# ask is completely gone, so go to next ask
					//				break
					break;
				}

				//			if bid.amount < ask.amount # bid is completely gone, slice the ask
				if (bid.amount < ask.amount) {
					//				sale.amount = bid.amount
					sale.amount = bid.amount;
					//				sales.append(sale)
					sales.push_back(sale);
					//				# update the ask
					//				# and allow further bids to process the remaining amount
					//				ask.amount = ask.amount - bid.amount
					ask.amount -= bid.amount;
					//				bid.amount = 0 # make sure the bid is not processed again
					bid.amount = 0;
					//				# some ask remains so go to the next bid
					//				continue
					continue;
				}
			}
		}
	}
    return sales;
}