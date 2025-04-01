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
class Boss : public Monster
{
private:
    std::string specialAbility;
    int specialAbility_damage;

public:
    Boss(const std::string &n, int h, int a, int d, const std::string& sa, int sa_d)
        : Monster(n, h, a, d), specialAbility(sa), specialAbility_damage(sa_d) {}
    void attackEnemy(Entity &target)
    {
        int damage = attack + specialAbility_damage;
        target.takeDamage(damage);
        std::cout << "Boss used Special Ability! It takes " << damage << " damage" << std::endl;
    }
};
int main()
{
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    // Создание объектов
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Monster dragon("Dragon", 150, 25, 20);
    Boss boss("Kailth", 300, 50, 20, "Fireball", 30);

        // Массив указателей на базовый класс
        Entity *entities[] = {&hero, &goblin, &dragon, &boss};

    // Полиморфное поведение
    for (auto &entity : entities)
    {
        entity->displayInfo(); // Вывод информации о сущности
    }

    // Бой между персонажем и монстрами
    hero.attackEnemy(goblin);
    goblin.attackEnemy(hero);
    dragon.attackEnemy(hero);
    hero.displayInfo();
    hero.heal(50);
    hero.displayInfo();
    boss.attackEnemy(hero);
    hero.displayInfo();

    return 0;
}
