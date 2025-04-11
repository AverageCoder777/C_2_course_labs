#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <iostream>

class Monster;

template<typename T>
class Logger {
private:
    std::ofstream log_file;

    std::string getTimestamp() const {
        std::time_t now = std::time(nullptr);
        std::string time = std::ctime(&now);
        if (!time.empty() && time.back() == '\n') {
            time.pop_back();
        }
        return time;
    }

public:
    Logger(const std::string& filename) {
        log_file.open(filename, std::ios::app);
        if (!log_file.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const T& message) {
        if (!log_file.is_open()) {
            throw std::runtime_error("Log file is not open");
        }
        log_file << "[" << getTimestamp() << "] " << message << "\n";
        log_file.flush();
    }
};

class Character
{
protected:
    std::string name;
    int hp;
    int max_hp;
    int attack;
    int defense;
    int level;
    int experience;

public:
    Character(const std::string &name, int hp = 100, int attack = 10, int defense = 5);
    void attackTarget(Monster& target);
    void heal(int amount);
    void gainExp(int exp);
    void displayInfo() const;

    int getDefense() const {return defense; }
    int getHp() const { return hp; }
    std::string getName() const { return name; }

    void setHp(int new_hp) {hp = new_hp; };
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);
};

class Monster
{
protected:
    std::string name;
    int hp;
    int attack;
    int defense;

public:
    Monster(const std::string &name, int hp, int attack, int defense);
    virtual ~Monster() = default;
    virtual void attackTarget(Character& target) = 0;
    virtual void displayInfo() const;

    int getDefense() const {return defense; }
    int getHp() const { return hp; }
    std::string getName() const { return name; }
    
    void setHp(int new_hp) {hp = new_hp; };
    virtual void save(std::ofstream &out) const;
    virtual void load(std::ifstream &in);
};

class Goblin : public Monster
{
public:
    Goblin();
    void attackTarget(Character &target) override;
};

class Dragon : public Monster
{
public:
    Dragon();
    void attackTarget(Character &target) override;
};

class Skeleton : public Monster
{
public:
    Skeleton();
    void attackTarget(Character&target) override;
};

template <typename T>
class Inventory
{
private:
    std::vector<T> items;

public:
    void addItem(const T &item)
    {
        items.push_back(item);
    }
    void removeItem(size_t index)
    {
        if (index >= items.size())
        {
            throw std::out_of_range("Invalid item index");
        }
        items.erase(items.begin() + index);
    }
    void display() const {
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i << ": " << items[i] << std::endl;
        }
    }
    void save(std::ofstream &out) const
    {
        out << items.size() << "\n";
        for (const auto &item : items)
        {
            out << item << "\n";
        }
    }
    void load(std::ifstream& in)
    {
        size_t size;
        in >> size;
        items.clear();
        std::string item;
        std::getline(in, item);
        for (size_t i = 0; i < size; ++i)
        {
            std::getline(in, item);
            items.push_back(item);
        }
    }
};

class Game
{
private:
    Character player;
    std::vector<std::unique_ptr<Monster>> monsters;
    Inventory<std::string> inventory;
    Logger<std::string> logger;

public:
    Game(const std::string& player_name);
    void start();
    void battle();
    void saveGame(const std::string& filename) const;
    void loadGame(const std::string& filename);
    void resetGame(const std::string& player_name);
};