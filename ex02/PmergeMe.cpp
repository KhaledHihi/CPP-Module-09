#include "PmergeMe.hpp"

bool isValideNumber(std::string arg)
{
    double toDouble;
    if (arg.empty() || (arg.find_first_not_of("0123456789+") != std::string::npos))
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
    for(int i = 1;i < ac; i++)
    {
        if(!isValideNumber(av[i]))
            throw std::invalid_argument("Error");
        int value = atoi(av[i]);
        if(seen.find(value) != seen.end())
            throw std::invalid_argument("Error");
        seen.insert(value);
        vc.push_back(value);
        dq.push_back(value);
    }
}

PmergeMe::PmergeMe(const PmergeMe& other)
{
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if(this != &other)
    {
        vc = other.vc;
        dq = other.dq;
    }
    return *this;
}
void    PmergeMe::displayBefore() const
{
    std::cout << "Before: ";
    for(size_t i = 0;i < vc.size();i++)
        std::cout << vc[i] << " ";
    std::cout << std::endl;
}

PmergeMe::~PmergeMe() {}

double PmergeMe::getCurrentTimeUs() const
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return static_cast<double>(time.tv_sec) * 1000000.0
         + static_cast<double>(time.tv_usec);
}

template <typename Container>
double PmergeMe::sortAndTime(Container &container)
{
    void(container); // don't forget to remove this line;
    double start = getCurrentTimeUs();
    double end = getCurrentTimeUs();
    return end - start;
}

template <typename Container>
void PmergeMe::checkSorted(const Container &container) const
{
    for(size_t i = 1;i < container(); i++)
    {
        if(container[i] < container[i - 1])
            throw std::runtime_error("Error");
    }
}

void PmergeMe::displayAfter() const
{
    std::cout << "After: ";
    for(size_t i = 0;i < vc.size();i++)
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

    double vectorTime = sortAndTime(vc);
    double dequeTime = sortAndTime(dq);

    checkSorted(vc);
    checkSorted(dq);

    displayAfter();
    displayTiming(vectorTime, dequeTime);
}