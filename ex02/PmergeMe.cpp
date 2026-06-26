#include "PmergeMe.hpp"

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

std::vector<size_t> PmergeMe::buildJacobsthalOrder(size_t size) const
{
    std::vector<size_t> order;
    std::vector<bool> used(size, false);

    size_t jacobPrev = 0;
    size_t jacobCurr = 1;

    while (jacobPrev < size)
    {
        size_t limit = jacobCurr;

        if (limit > size)
            limit = size;

        for (size_t i = limit; i > jacobPrev; i--)
        {
            if (!used[i - 1])
            {
                order.push_back(i - 1);
                used[i - 1] = true;
            }
        }

        size_t next = jacobCurr + 2 * jacobPrev;
        jacobPrev = jacobCurr;
        jacobCurr = next;

        if (jacobCurr == 0)
            break;
    }

    for (size_t i = 0; i < size; i++)
    {
        if (!used[i])
            order.push_back(i);
    }

    return order;
}

template <typename Container>
void PmergeMe::mergeInsertSort(Container &container)
{
    typedef typename Container::value_type Value;

    if (container.size() < 2)
        return;

    Container mainChain;
    std::vector<Value> pending;

    bool hasOdd = false;
    Value oddValue = Value();

    size_t i = 0;
    while (i + 1 < container.size())
    {
        Value first = container[i];
        Value second = container[i + 1];

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
        Value value = pending[order[j]];
        typename Container::iterator pos =
            std::lower_bound(mainChain.begin(), mainChain.end(), value);
        mainChain.insert(pos, value);
    }

    if (hasOdd)
    {
        typename Container::iterator pos =
            std::lower_bound(mainChain.begin(), mainChain.end(), oddValue);
        mainChain.insert(pos, oddValue);
    }

    container.clear();
    container.insert(container.end(), mainChain.begin(), mainChain.end());
}

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
    double start = getCurrentTimeUs();
    mergeInsertSort(container);
    double end = getCurrentTimeUs();
    return end - start;
}

template <typename Container>
void PmergeMe::checkSorted(const Container &the_container) const
{
    for(size_t i = 1;i < the_container.size(); i++)
    {
        if(the_container[i] < the_container[i - 1])
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