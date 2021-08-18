//
// Created by Austin Clyde on 8/17/21.
//

#ifndef TRADINGRUNNER_MESSAGE_HPP
#define TRADINGRUNNER_MESSAGE_HPP

#include <vector>
#include <streambuf>
#include <utility>
#include <sstream>

struct CBMessage {

    CBMessage() = default;

    virtual void parse(const std::string &data) {};
};

struct SnapshotMessage : CBMessage {
    SnapshotMessage() = default;

    std::vector<double> buy_prices;
    std::vector<double> buy_quantities;
    std::vector<double> sell_prices;
    std::vector<double> sell_quantities;

    void parse(const std::string &data) override;

};


void SnapshotMessage::parse(const std::string &data) {
    // TODO
}


#endif //TRADINGRUNNER_MESSAGE_HPP
