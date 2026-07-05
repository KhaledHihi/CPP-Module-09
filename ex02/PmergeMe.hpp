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

class PmergeMe
{
private:
    std::vector<int> vc;
    std::deque<int> dq;

public:
    PmergeMe();
    PmergeMe(int ac, char **av);
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    bool isValidNumber(const char *str) const;
    double getCurrentTimeUs() const;

    void parseInput(int ac, char **av);
    void displayBefore() const;
    void displayAfter() const;
    void displayTiming(double vectorTime, double dequeTime) const;
    void displayComparisons(size_t vectorComparisons, size_t dequeComparisons) const;
    void run();

    std::vector<size_t> buildJacobsthalOrder(size_t size) const;
    size_t getComparisonCount(size_t size) const;

    template <typename Container>
    void mergeInsertSort(Container &container, size_t &comparisonCount);

    template <typename Container>
    double sortAndTime(Container &container, size_t &comparisonCount);

    template <typename Container>
    void checkSorted(const Container &container) const;
};

#endif