#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Weapon{
private:
    std::string name;
    int attack;
    int weight;
public:
    Weapon(const std::string n, int a, int w):
    name(n),attack(a),weight(w){
        std::cout << "New weapon with name " << name <<" created!" << std::endl;
    }
    ~Weapon(){
        std::cout << "Weapon with name " << name <<" was destroyed!" << std::endl;
    }
    void displayInfo() const{
        std::cout<<"Name: "<<name<<", Damage: "<<attack<<", Weight: "<<weight<<std::endl;
    }
};
int main()
{
    Weapon AK ("AK-47",35,2000);
    Weapon Machete ("Machete",50,500);
    AK.displayInfo();
    Machete.displayInfo();
    return 0;
}
