//
// Created by aclyde on 8/17/21.
//

#ifndef TRADINGRUNNER_TRADER_HPP
#define TRADINGRUNNER_TRADER_HPP

#include <fstream>
#include <iostream>
#include "../Orderbook/OrderBook.hpp"
#include "../Orderbook/Message.hpp"

struct Bot {
    OrderBook book;

    Bot() = default;

    // Here, messages come in, and you need to parse them into the correct type, update the order book
    void message_in(std::string const &str);

    void parse_snapshot_msg(const std::string &str);
};

// USE ME FOR PART I, PASS ME THE STRING YOU GET FROM READING IN THE FILE MESSAGES
void Bot::parse_snapshot_msg(const std::string &str) {
    // TODO
    std::cout << "BOT::parse_snapshot_msg: " << str << std::endl;
}

//USE ME FOR PART II, and move over the functionality from parse_snapshot_msg
void Bot::message_in(const std::string &str) {
    std::cout << "RECEIVED THE MESSAGE" << str << std::endl;

    // you should check the "type" field of the string
    // make the correct Message type object
    // pass that message type object to the order book or ignore it
    // store and save the time elapsed since the last time the book was printed
}

#endif //TRADINGRUNNER_TRADER_HPP
