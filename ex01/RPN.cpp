#include "RPN.hpp"

bool parse_arg(char *arg)
{
    for (int i = 0; arg[i] != '\0'; i++){
        if (arg[i] <= 32)
            continue;
        if ((arg[i] < '0' || arg[i] > '9') && (arg[i] != '*' && arg[i] != '+' && arg[i] != '-' && arg[i] != '/'))
            return (std::cerr << "Error" << std::endl, 1);
        if (arg[i + 1] != '\0' && arg[i + 1] != ' ')
            return (std::cout << "Error" << std::endl, 1);
        if (arg[i + 1] == ' ')
            i++;
    }
    return 0;
}