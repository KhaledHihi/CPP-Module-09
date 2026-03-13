#include "BitcoinExchange.hpp"

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open database file " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos)
        {
            std::string date = line.substr(0, commaPos);
            float rate = std::stof(line.substr(commaPos + 1));
            _rates[date] = rate;
        }
    }
    file.close();
}