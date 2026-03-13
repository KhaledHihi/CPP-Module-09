#pragma once


#include <map>
#include <iostream>
#include <fstream>

class BitcoinExchange
{
    private:
        std::map<std::string, float> _rates;

    public:
        BitcoinExchange();
        ~BitcoinExchange();

        void loadDatabase(const std::string& filename);
        void processInput(const std::string& filename);
};