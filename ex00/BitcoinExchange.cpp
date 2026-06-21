#include "BitcoinExchange.hpp"

bool    handle_date(std::string &date, std::string &line)
{
     int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());

    if (date < "2009-01-03")
	    return (std::cerr << "Error: btc did not exist in this date => " << line << std::endl, false);
    bool leap = false;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        leap = true;
    if (month > 12 || month < 1 || day < 1)
        return (std::cerr << "Error: bad input => " << line << std::endl, false);
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        if (day > 31)
            return (std::cerr << "Error: bad input => " << line << std::endl, false);
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
            return (std::cerr << "Error: bad input => " << line << std::endl, false);
    }
    else if (month == 2)
    {
        if ((day > 29 && leap) || (!leap && day > 28))
            return (std::cerr << "Error: bad input => " << line << std::endl, false);
    }
    line = date;
    return true;
}
bool    processInput(std::string &line, double &val){
    size_t pos;
    std::string date;
    std::string value;
    char *end = NULL;

	pos = line.find(" | ");
    if (pos == std::string::npos)
        return (std::cerr << "Error: bad input => " << line << std::endl, false);
    date = line.substr(0, pos);
    value = line.substr(pos + 3, line.size() - pos);
    val = std::strtod(value.c_str(), &end);
    if (val > 1000)
        return (std::cerr << "Error: too large a number." << std::endl, false);
    if (val < 0)
        return (std::cerr << "Error: not a positive number." << std::endl, false);
    if (*end != '\0' || isspace(value[0]))
        return (std::cerr << "Error: bad input => " << line << std::endl, false);
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return (std::cerr << "Error: bad input => " << line << std::endl, false);
    for (size_t i = 0; date[i]; i++)
    {
        if (i == 4 || i == 7)
            continue;
        else if (date[i] < '0' || date[i] > '9')
            return (std::cerr << "Error: bad input => " << line << std::endl, false);
    }
   return (handle_date(date, line));
}

void    print_the_result(std::map<std::string, float> &data ,std::string &date, double &value){

    if (data.size() == 0)
    {
        std::cerr << "Error the Data is empty." << std::endl;
        return ;
    }
    std::map<std::string, float>::iterator it = data.lower_bound(date);
    if (it == data.end())
        --it;
    else if (it != data.begin() && it->first.compare(date))
        --it;
    std::cout << date << " => " << value << " = " << value * it->second << std::endl;
}