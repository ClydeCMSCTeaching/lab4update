//
// Created by Austin Clyde on 8/17/21.
//

//
// Created by Austin Clyde on 8/17/21.
//

#include <iostream>
#include <streambuf>
#include <utility>

#include "Trader/Trader.hpp"

std::vector<std::string> readFile(std::string const &fname) {
    std::ifstream t(fname);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());

    //TODO split all the messages. (hint: they are sep by new line)

    return std::vector<std::string>(); //this is just an empty list
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: ./OrderBook orderbooksnapshot.txt\n";
        std::cout << "Check you called this with a single command line arg.\n";
        std::cout << "Exiting...\n";
        return 0;
    }

    std::vector<std::string> file_data = readFile(argv[1]);
    Bot b;

    for (auto const &msg : file_data)
        b.message_in(msg);

}