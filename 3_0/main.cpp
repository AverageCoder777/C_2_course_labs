#include <iostream>
#include <string>

class Character
{
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string &n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Перегрузка оператора ==
    bool operator==(const Character &other) const
    {
        return name == other.name && health == other.health;
    }

    // Перегрузка оператора <<
    friend std::ostream &operator<<(std::ostream &os, const Character &character)
    {
        os << "Character: " << character.name << ", HP: " << character.health
           << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};
class Weapon
{
private:
    std::string name;
    int attack;
    int weight;

public:
    Weapon(const std::string n, int a, int w) : name(n), attack(a), weight(w)
    {
        std::cout << "New weapon with name " << name << " created!" << std::endl;
    }
    ~Weapon()
    {
        std::cout << "Weapon with name " << name << " was destroyed!" << std::endl;
    }
    void displayInfo() const
    {
        std::cout << "Name: " << name << ", Damage: " << attack << ", Weight: " << weight << std::endl;
    }
    Weapon operator+(const Weapon& other) const{
        std::string newName = this->name + " + " + other.name;
        int attack = this->attack + other.attack;
        int weight = this->weight + other.weight;
        return Weapon(newName, attack, weight);
    }
    bool operator>(const Weapon& other) const{
        return attack == other.attack;
    }
    std::string getName() const{
        return name;
    }
};
int main()
{
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);
    Weapon AK ("AK-47",35,2000);
    Weapon Machete ("Machete",50,500);
    Weapon RPG ("RPG-7",85,2500);
    if (hero1 == hero2)
    {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3))
    {
        std::cout << "Hero1 and Hero3 are different!\n";
    }
    std::cout << hero1 << std::endl; // Вывод информации о персонаже
    Weapon HandmadeWeapon = AK+Machete;
    HandmadeWeapon.displayInfo();
    if (HandmadeWeapon > AK)
    {
        std::cout<<"Damage of " <<HandmadeWeapon.getName() << " and " << AK.getName() << " are the same!\n";
    }
    else{
        std::cout<<"Damage of " <<HandmadeWeapon.getName() << " and " << AK.getName() << " are different!\n";
    }
    if (HandmadeWeapon > RPG)
    {
        std::cout<<"Damage of " <<HandmadeWeapon.getName() << " and " << RPG.getName() << " are the same!\n";
    }
    else{
        std::cout<<"Damage of " <<HandmadeWeapon.getName() << " and " << RPG.getName() << " are different!\n";
    }
    return 0;
}
