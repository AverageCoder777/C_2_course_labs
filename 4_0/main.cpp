#include <memory>
#include <string>
#include <iostream>
class Inventory {
    private:
        std::string* items;
        size_t capacity;
        size_t currentSize;
    
    public:
        Inventory(size_t initialCapacity = 10) 
            : capacity(initialCapacity), currentSize(0) {
            items = new std::string[capacity];
        }

        ~Inventory() {
            delete[] items;
        }

        void addItem(const std::string& item) {
            if (currentSize < capacity) {
                items[currentSize] = item;
                currentSize++;
            } else {
                size_t newCapacity = capacity * 2;
                std::string* newItems = new std::string[newCapacity];
                for (size_t i = 0; i < currentSize; i++) {
                    newItems[i] = std::move(items[i]);
                }

                newItems[currentSize] = item;
                currentSize++;

                delete[] items;
                items = newItems;
                capacity = newCapacity;
            }
        }
        void displayInventory() const {
            if (currentSize == 0) {
                std::cout << "Inventory is empty\n";
                return;
            }
            std::cout << "Inventory contents:\n";
            for (size_t i = 0; i < currentSize; i++) {
                std::cout << i + 1 << ". " << items[i] << "\n";
            }
        }
    };
int main() {
    std::unique_ptr<Inventory>  inventories[] = {
        std::make_unique<Inventory>(3),
        std::make_unique<Inventory>(2)
    };
    inventories[0]->addItem("Sword");
    inventories[0]->addItem("Shield");
    inventories[0]->addItem("Potion");
    inventories[0]->addItem("Ring");


    inventories[1]->addItem("Staff");
    inventories[1]->addItem("Robe");
    for (const auto& inv : inventories) {
        inv->displayInventory();
        std::cout << "----------------\n";
    }
    return 0;
}
