#include "PmergeMe.hpp"

size_t g_comparisons = 0;

template <typename Iterator, typename T>
Iterator countedLowerBound(Iterator first, Iterator last, const T &value)
{
    while (first < last)
    {
        Iterator middle = first + (last - first) / 2;
        ++g_comparisons;
        if (*middle < value)
            first = middle + 1;
        else
            last = middle;
    }
    return first;
}

bool isValideNumber(std::string arg)
{
    double toDouble;
    if (arg.empty() || (arg.find_first_not_of("0123456789") != std::string::npos))
        return false;
    toDouble = std::strtod(arg.c_str(), 0);
    if (toDouble > std::numeric_limits<int>::max())
        return false;
    return true;
}

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(int ac, char **av)
{
    std::set<int> seen;
    if (ac <= 1)
        throw std::invalid_argument("Error");
    for (int i = 1; i < ac; i++)
    {
        if (!isValideNumber(av[i]))
            throw std::invalid_argument("Error");
        int value = atoi(av[i]);
        if (seen.find(value) != seen.end())
            throw std::invalid_argument("Error");
        seen.insert(value);
        vc.push_back(value);
        dq.push_back(value);
    }
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    *this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        vc = other.vc;
        dq = other.dq;
    }
    return *this;
}
void PmergeMe::displayBefore() const
{
    std::cout << "Before: ";
    for (size_t i = 0; i < vc.size(); i++)
        std::cout << vc[i] << " ";
    std::cout << std::endl;
}

PmergeMe::~PmergeMe() {}

std::vector<size_t> PmergeMe::buildJacobsthalOrder(size_t n) const
{
    std::vector<size_t> order;

    if (n == 0)
        return order;
    order.push_back(0);

    size_t prev = 1;
    size_t curr = 1;
    size_t last = 1;

    while (last < n)
    {
        size_t limit = curr;
        if (limit > n)
            limit = n;

        for (size_t i = limit; i > last; i--)
            order.push_back(i - 1);

        last = limit;
        size_t next = curr + 2 * prev;
        prev = curr;
        curr = next;
    }

    return order;
}

template <typename Container>
void PmergeMe::mergeInsertSort(Container &container)
{
    if (container.size() < 2)
        return;

    Container mainChain;
    std::vector<int> pending;

    bool hasOdd = false;
    int oddValue = int();

    size_t i = 0;
    while (i + 1 < container.size())
    {
        int first = container[i];
        int second = container[i + 1];
        ++g_comparisons;
        if (second < first)
        {
            mainChain.push_back(first);
            pending.push_back(second);
        }
        else
        {
            mainChain.push_back(second);
            pending.push_back(first);
        }
        i += 2;
    }

    if (i < container.size())
    {
        hasOdd = true;
        oddValue = container[i];
    }

    mergeInsertSort(mainChain);

    std::vector<size_t> order = buildJacobsthalOrder(pending.size());

    for (size_t j = 0; j < order.size(); j++)
    {
        size_t idx = order[j];
        int value = pending[idx];
        typename Container::iterator pos = countedLowerBound(mainChain.begin(), mainChain.end(), value);
        mainChain.insert(pos, value);
    }

    if (hasOdd)
    {
        typename Container::iterator pos = countedLowerBound(mainChain.begin(), mainChain.end(), oddValue);
        mainChain.insert(pos, oddValue);
    }

    container.clear();
    container.insert(container.end(), mainChain.begin(), mainChain.end());
}

double PmergeMe::getCurrentTimeUs() const
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return static_cast<double>(time.tv_sec) * 1000000.0 + static_cast<double>(time.tv_usec);
}

template <typename Container>
double PmergeMe::sortAndTime(Container &container)
{
    double start = getCurrentTimeUs();
    mergeInsertSort(container);
    double end = getCurrentTimeUs();
    return end - start;
}

template <typename Container>
void PmergeMe::checkSorted(const Container &the_container) const
{
    for (size_t i = 1; i < the_container.size(); i++)
    {
        if (the_container[i] < the_container[i - 1])
            throw std::runtime_error("Error");
    }
}

void PmergeMe::displayAfter() const
{
    std::cout << "After: ";
    for (size_t i = 0; i < vc.size(); i++)
        std::cout << vc[i] << " ";
    std::cout << std::endl;
}

void PmergeMe::displayTiming(double vectorTime, double dequeTime) const
{
    std::cout << "Time to process a range of " << vc.size()
              << " elements with std::vector : "
              << vectorTime << " us" << std::endl;
    std::cout << "Time to process a range of " << dq.size()
              << " elements with std::deque : "
              << dequeTime << " us" << std::endl;
}

void PmergeMe::run()
{
    displayBefore();

    g_comparisons = 0;
    double vectorTime = sortAndTime(vc);
    std::cout << "Number of comparisons with std::vector : " << g_comparisons << std::endl;

    g_comparisons = 0;
    double dequeTime = sortAndTime(dq);
    std::cout << "Number of comparisons with std::deque : " << g_comparisons << std::endl;

    checkSorted(vc);
    checkSorted(dq);

    displayAfter();
    displayTiming(vectorTime, dequeTime);
}