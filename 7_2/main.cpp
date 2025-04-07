#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>

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
    std::string getName() const { return name; }
    void takeDamage(int damage) { health -= damage; }
    bool isAlive() const { return health > 0; }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const
    {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Виртуальный деструктор
    virtual ~Entity() {}
};

class Monster : public Entity
{
public:
    Monster(const std::string &n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    // Переопределение метода attack
    void attackEnemy(Entity& target) override
    {
        int damage = attack - target.getDefence();
        if (damage > 0)
        {
            // Шанс на ядовитую атаку (30%)
            if (rand() % 100 < 30)
            {
                damage += 5; // Дополнительный урон от яда
                std::cout << "Poisonous attack! ";
            }
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else
        {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override
    {
        std::cout << "Monster: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
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

std::vector<Monster> monsters;
std::mutex monstersMutex;
std::mutex fightMutex;

void generateMonsters() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Новый монстр каждые 3 секунды
        std::lock_guard<std::mutex> lock(monstersMutex);
        monsters.push_back(Monster("Goblin_" + std::to_string(rand()%1000), 50, 15, 5));
        std::cout << "New monster generated!\n";
    }
}
void fight(Character &hero, Monster& monster){
    std::cout<<"\nBattle begins between "<<hero.getName()<<" and "<<monster.getName()<<"!\n";
    while (hero.isAlive() && monster.isAlive()) {
        std::lock_guard<std::mutex> lock(fightMutex);
        
        // Ход героя
        hero.attackEnemy(monster);
        if (!monster.isAlive()) {
            std::cout << monster.getName() << " has been defeated!\n";
            break;
        }

        // Ход монстра
        monster.attackEnemy(hero);
        if (!hero.isAlive()) {
            std::cout << hero.getName() << " has been defeated!\n";
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    if (hero.isAlive()) {
        hero.heal(20); // Герой восстанавливает здоровье после победы
    }
}

int main() {
    Character hero("Hero", 100, 20, 10);

    std::thread monsterGenerator(generateMonsters);
    monsterGenerator.detach(); // Отсоединяем поток

    while (hero.isAlive()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(monstersMutex);
        if (!monsters.empty()){
            Monster currentMonster = monsters[0];
            monsters.erase(monsters.begin());
            std::thread fightThread(fight, std::ref(hero), std::ref(currentMonster));
            fightThread.join();
            std::cout << "\nCurrent status:\n";
            hero.displayInfo();
            std::cout << "Monsters remaining: " << monsters.size() << "\n";
        }
    }
    std::cout << "Game Over!\n";
    return 0;
}
