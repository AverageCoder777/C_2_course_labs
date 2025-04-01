#include <iostream>
#include <vector>
#include <string>

template <typename T>
class Queue
{
private:
    std::vector<T> items;

public:
    void push(const T &item)
    {
        items.push_back(item);
    }
    T pop()
    {
        if (items.size() == 0)
        {
            throw std::invalid_argument("Queue is empty");
        }
        T frontItem = items[0];
        items.erase(items.begin());
        return frontItem;
    }
    void display() const
    {
        for (const auto &item : items)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main()
{
    try
    {
        std::cout << "Testing empty queue:\n";
        Queue<std::string> emptyQueue;
        std::cout << "Initial queue: ";
        emptyQueue.display();
        emptyQueue.pop();
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
    try
    {

        std::cout << "\nTesting Queue with string:\n";
        Queue<std::string> stringQueue;
        stringQueue.push("Filler");
        std::cout << "Initial queue: ";
        stringQueue.display();
        stringQueue.pop();
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}