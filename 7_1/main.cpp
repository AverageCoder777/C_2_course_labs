#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

class Entity
{
protected:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Entity(const std::string &n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Виртуальный метод для атаки
    virtual void attackEnemy(Entity &target)
    {
        int damage = attack - target.defense;
        if (damage > 0)
        {
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else
        {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }
    virtual void heal(int amount){
        health +=amount;
    }
    //Геттер для защиты и имени, т.к. поля протектед, а во всех переопределениях функции attackEnemy за target взят класс Entity
    int getDefence() const { return defense; }
    int getHealth() const {return health;}
    int getAttack() const {return attack;}
    std::string getName() const { return name; }

    void takeDamage(int damage) { health -= damage; }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const
    {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Виртуальный деструктор
    virtual ~Entity() {}
};

class Character : public Entity
{
public:
    Character(const std::string &n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    // Переопределение метода attack
    void attackEnemy(Entity& target) override
    {
        int damage = attack - target.getDefence();
        if (damage > 0)
        {
            // Шанс на критический удар (20%)
            if (rand() % 100 < 20)
            {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else
        {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }
    void heal(int amount){
        Entity::heal(amount);
        std::cout<<"Character healed with "<< amount<< " HP"<<std::endl;
    }

    // Переопределение метода displayInfo
    void displayInfo() const override
    {
        std::cout << "Character: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};
template <typename T>
class GameManager
{
    private: std::vector<T> entities;
public:
    void addEntity(T entity)
    {
        if (entity->getHealth() <= 0)
        {
            throw std::invalid_argument("Entity has invalid health");
        }
        entities.push_back(entity);
    }
    void saveToFile(const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file for writing.");
        }
        for (const auto& entity : entities) {
            file << entity->getName() << " " << entity->getHealth() << " "
                 << entity->getAttack() << " " << entity->getDefence() << "\n";
        }
        file.close();
    }

    void loadFromFile(GameManager<Entity *> &manager, const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file for reading.");
        }
        std::string name;
        int health, attack, defense;
        while (file >> name >> health >> attack >> defense) {
            entities.push_back(new Character(name, health, attack, defense));
        }
        file.close();
    }
    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }
    ~GameManager() {
        for (auto entity : entities) {
            delete entity;  // Освобождаем память
        }
    }
};

int main()
{
    try{
    GameManager<Entity *> manager;
    manager.addEntity(new Character("Hero", 100, 20,10));
    manager.saveToFile("game_save.txt");
    std::cout << "Game saved successfully.\n";

    GameManager<Entity *> loadedManager;
    loadedManager.loadFromFile(loadedManager, "game_save.txt");
    std::cout << "Loaded entities:\n";
    loadedManager.displayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
