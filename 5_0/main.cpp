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
    std::cout << "Testing Queue with strings:\n";
    Queue<std::string> stringQueue;
    stringQueue.push("Apple");
    stringQueue.push("Banana");
    stringQueue.push("Cherry");

    std::cout << "Initial queue: ";
    stringQueue.display();
    std::cout << "Popped: " << stringQueue.pop() << std::endl;
    std::cout << "After pop: ";
    stringQueue.display();

    std::cout << "Testing Queue with integers:\n";
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "Initial queue: ";
    intQueue.display();
    std::cout << "Popped: " << intQueue.pop() << std::endl;
    std::cout << "After pop: ";
    intQueue.display();
    return 0;
}