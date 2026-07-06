#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <limits>
#include <sys/time.h>
#include <set>

struct Node
{
    int value;
    size_t id;
};

struct PendingNode
{
    Node small;
    size_t bigId;
};

class PmergeMe
{
private:
    std::vector<Node> vc;
    std::deque<Node> dq;

public:
    PmergeMe();
    PmergeMe(int ac, char **av);
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void displayBefore() const;
    void displayAfter() const;
    void displayTiming(double vectorTime, double dequeTime) const;
    void run();

    double getCurrentTimeUs() const;
    std::vector<size_t> buildJacobsthalOrder(size_t n) const;

    template <typename Container>
    void mergeInsertSort(Container &container);

    template <typename Container>
    double sortAndTime(Container &container);

    template <typename Container>
    void checkSorted(const Container &container) const;
};

bool isValideNumber(std::string arg);

#endif