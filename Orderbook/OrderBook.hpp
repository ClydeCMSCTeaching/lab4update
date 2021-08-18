//
// Created by aclyde on 8/17/21.
//

#ifndef TRADINGRUNNER_ORDERBOOK_HPP
#define TRADINGRUNNER_ORDERBOOK_HPP

#include <iomanip>
#include <streambuf>
#include <utility>
#include <vector>
#include <iostream>
#include "Message.hpp"

struct OrderBook {
    OrderBook() = default;
    void printToScreen();

    // you need to add data to hold the order book.. you need to decide on what data structure to use and what functions to add
};


void OrderBook::printToScreen() {
    //TODO
}


#endif //TRADINGRUNNER_ORDERBOOK_HPP
