#pragma once

#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>

class PmergeMe
{
private:
    std::vector<int> _vector;
    std::deque<int> _deque;

public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void parseInput(int ac, char **av);
    void run();
};