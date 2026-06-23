#include "RPN.hpp"

static bool is_operator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int main(int ac, char **av)
{
    if (ac != 2)
        return (std::cerr << "Error" << std::endl, 1);

    if (parse_arg(av[1]))
        return 1;

    std::stack<int> calc;
    int a;
    int b;

    for (int i = 0; av[1][i]; i++)
    {
        if (av[1][i] <= 32)
            continue;

        if (av[1][i] >= '0' && av[1][i] <= '9')
            calc.push(av[1][i] - '0');
        else if (is_operator(av[1][i]))
        {
            if (calc.size() < 2)
                return (std::cerr << "Error" << std::endl, 1);

            a = calc.top();
            calc.pop();

            b = calc.top();
            calc.pop();

            if (av[1][i] == '*')
                calc.push(b * a);
            else if (av[1][i] == '+')
                calc.push(b + a);
            else if (av[1][i] == '-')
                calc.push(b - a);
            else if (av[1][i] == '/')
            {
                if (a == 0)
                    return (std::cerr << "Error" << std::endl, 1);
                calc.push(b / a);
            }
        }
        else
            return (std::cerr << "Error" << std::endl, 1);
    }

    if (calc.size() != 1)
        return (std::cerr << "Error" << std::endl, 1);

    std::cout << calc.top() << std::endl;
    return 0;
}