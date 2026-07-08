#include "PmergeMe.hpp"

size_t g_comparisons = 0;

bool isValideNumber(std::string arg)
{
    double toDouble;
    if (arg.empty() || (arg.find_first_not_of("0123456789") != std::string::npos))
        return false;
    toDouble = std::strtod(arg.c_str(), 0);
    if (toDouble <= 0 || toDouble > std::numeric_limits<int>::max())
        return false;
    return true;
}

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(int ac, char **av)
{
    std::set<int> seen;

    if (ac <= 1)
        throw std::invalid_argument("Error");

    for (int i = 1; i < ac; i++)
    {
        if (!isValideNumber(av[i]))
            throw std::invalid_argument("Error");

        int value = std::atoi(av[i]);

        if (seen.find(value) != seen.end())
            throw std::invalid_argument("Error");

        seen.insert(value);

        Node node;
        node.value = value;
        node.id = static_cast<size_t>(i - 1);

        vc.push_back(node);
        dq.push_back(node);
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

PmergeMe::~PmergeMe()
{
}

void PmergeMe::displayBefore() const
{
    std::cout << "Before: ";
    for (size_t i = 0; i < vc.size(); i++)
        std::cout << vc[i].value << " ";
    std::cout << std::endl;
}

void PmergeMe::displayAfter() const
{
    std::cout << "After: ";
    for (size_t i = 0; i < vc.size(); i++)
        std::cout << vc[i].value << " ";
    std::cout << std::endl;
}

template <typename Iterator>
Iterator countedLowerBound(Iterator first, Iterator last, int value)
{
    while (first < last)
    {
        Iterator middle = first + (last - first) / 2;

        ++g_comparisons;
        if (middle->value < value)
            first = middle + 1;
        else
            last = middle;
    }
    return first;
}

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
    std::vector<PendingNode> pending;

    bool hasOdd = false;
    Node oddValue;

    size_t i = 0;
    while (i + 1 < container.size())
    {
        Node first = container[i];
        Node second = container[i + 1];

        ++g_comparisons;
        if (second.value < first.value)
        {
            mainChain.push_back(first);

            PendingNode p;
            p.small = second;
            p.bigId = first.id;
            pending.push_back(p);
        }
        else
        {
            mainChain.push_back(second);

            PendingNode p;
            p.small = first;
            p.bigId = second.id;
            pending.push_back(p);
        }

        i += 2;
    }

    if (i < container.size())
    {
        hasOdd = true;
        oddValue = container[i];
    }

    mergeInsertSort(mainChain);

    std::vector<PendingNode> sortedPending;

    for (size_t i = 0; i < mainChain.size(); i++)
    {
        for (size_t j = 0; j < pending.size(); j++)
        {
            if (mainChain[i].id == pending[j].bigId)
            {
                sortedPending.push_back(pending[j]);
                break;
            }
        }
    }

    std::vector<size_t> order = buildJacobsthalOrder(sortedPending.size());

    for (size_t j = 0; j < order.size(); j++)
    {
        size_t idx = order[j];

        Node value = sortedPending[idx].small;
        size_t bigId = sortedPending[idx].bigId;

        typename Container::iterator bound = mainChain.begin();

        while (bound != mainChain.end() && bound->id != bigId)
            ++bound;

        typename Container::iterator pos =
            countedLowerBound(mainChain.begin(), bound, value.value);

        mainChain.insert(pos, value);
    }

    if (hasOdd)
    {
        typename Container::iterator pos =
            countedLowerBound(mainChain.begin(), mainChain.end(), oddValue.value);
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
void PmergeMe::checkSorted(const Container &container) const
{
    for (size_t i = 1; i < container.size(); i++)
    {
        if (container[i].value < container[i - 1].value)
            throw std::runtime_error("Error");
    }
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
    std::cout << "Number of comparisons with std::vector : "
              << g_comparisons << std::endl;

    g_comparisons = 0;
    double dequeTime = sortAndTime(dq);
    std::cout << "Number of comparisons with std::deque : "
              << g_comparisons << std::endl;

    checkSorted(vc);
    checkSorted(dq);

    displayAfter();
    displayTiming(vectorTime, dequeTime);
}