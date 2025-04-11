#include "Base_classes.h"
#include <random>
#include <ctime>

// Переопределение персонажа
Character::Character(const std::string &name, int hp, int attack, int defense)
    : name(name), hp(hp), max_hp(hp), attack(attack), defense(defense), level(1), experience(0) {}
void Character::attackTarget(Monster &target)
{
    int damage = std::max(1, attack - target.getDefense());
    target.setHp(std::max(0, target.getHp() - damage));
    std::cout << name << " deals " << damage << " damage to " << target.getName() << std::endl;
}

void Character::heal(int amount)
{
    hp = std::min(max_hp, hp + amount);
    std::cout << name << " heals for " << amount << " HP" << std::endl;
}

void Character::gainExp(int exp)
{
    experience += exp;
    while (experience >= level * 100)
    {
        experience -= level * 100;
        level++;
        max_hp += 20;
        hp = max_hp;
        attack += 5;
        defense += 2;
        std::cout << name << " leveled up to " << level << "!" << std::endl;
    }
}

void Character::displayInfo() const
{
    std::cout << "Name: " << name << "\nHP: " << hp << "/" << max_hp
              << "\nLevel: " << level << "\nEXP: " << experience
              << "\nAttack: " << attack << "\nDefense: " << defense << std::endl;
}

void Character::save(std::ofstream &out) const
{
    out << name << "\n"
        << hp << " " << max_hp << " " << attack << " "
        << defense << " " << level << " " << experience << "\n";
}

void Character::load(std::ifstream &in)
{
    std::getline(in, name);
    in >> hp >> max_hp >> attack >> defense >> level >> experience;
}

// Переопределение монстров
Monster::Monster(const std::string &name, int hp, int attack, int defense)
    : name(name), hp(hp), attack(attack), defense(defense) {}

void Monster::displayInfo() const
{
    std::cout << "Monster: " << name << "\nHP: " << hp
              << "\nAttack: " << attack << "\nDefense: " << defense << std::endl;
}

void Monster::save(std::ofstream &out) const
{
    out << name << "\n"
        << hp << " " << attack << " " << defense << "\n";
}

void Monster::load(std::ifstream &in)
{
    std::getline(in, name);
    in >> hp >> attack >> defense;
}

// Переопределение гоблина
Goblin::Goblin() : Monster("Goblin", 50, 8, 3) {}

void Goblin::attackTarget(Character &target)
{
    int damage = std::max(1, attack - target.getDefense() / 10);
    target.setHp(std::max(0, target.getHp() - damage));
    std::cout << name << " deals " << damage << " damage to " << target.getName() << std::endl;
}

// Переопределение дракона
Dragon::Dragon() : Monster("Dragon", 200, 20, 15) {}

void Dragon::attackTarget(Character &target)
{
    int damage = std::max(1, attack - target.getDefense() / 15);
    target.setHp(std::max(0, target.getHp() - damage));
    std::cout << name << " deals " << damage << " damage to " << target.getName() << std::endl;
}

// Переопределение скелета
Skeleton::Skeleton() : Monster("Skeleton", 80, 12, 8) {}

void Skeleton::attackTarget(Character &target)
{
    int damage = std::max(1, attack - target.getDefense() / 12);
    target.setHp(std::max(0, target.getHp() - damage));
    std::cout << name << " deals " << damage << " damage to " << target.getName() << std::endl;
}

// Переопределение игры
Game::Game(const std::string &player_name) : player(player_name), logger("game_log.txt")
{
    monsters.push_back(std::make_unique<Goblin>());
    monsters.push_back(std::make_unique<Dragon>());
    monsters.push_back(std::make_unique<Skeleton>());
    logger.log("Game started by: " + player_name);
}

void Game::resetGame(const std::string &player_name)
{
    player = Character(player_name);
    monsters.clear();
    monsters.push_back(std::make_unique<Goblin>());
    monsters.push_back(std::make_unique<Dragon>());
    monsters.push_back(std::make_unique<Skeleton>());
    inventory = Inventory<std::string>();
    logger.log("New game started for player: " + player_name);
}

void Game::start()
{
    std::cout << "Welcome to the RPG Game!" << std::endl;
    bool game_running = true;
    while (game_running)
    {
        std::cout << "\n1. Battle\n2. Show Stats\n3. Heal\n4. Inventory\n5. Save\n6. Load\n7. Exit\n";
        int choice;
        std::cin >> choice;
        try
        {
            switch (choice)
            {
            case 1:
                logger.log("Go battle");
                battle();
                if (player.getHp() <= 0)
                {
                    std::cout << "Would you like to start a new game? (1: Yes, 2: No)\n";
                    int new_game_choice;
                    std::cin >> new_game_choice;
                    if (new_game_choice == 1)
                    {
                        std::cout << "Enter new player name: ";
                        std::string new_name;
                        std::cin.ignore();
                        std::getline(std::cin, new_name);
                        resetGame(new_name);
                        logger.log("Player chose to start a new game");
                        std::cout << "New game started!\n";
                    }
                    else
                    {
                        logger.log("Player chose to exit after game over");
                        game_running = false;
                    }
                }
                break;
            case 2:
                player.displayInfo();
                logger.log("View stats");
                break;
            case 3:
                player.heal(20);
                logger.log("Heal with 20 Hp");
                break;
            case 4:
                inventory.display();
                logger.log("View inventory");
                break;
            case 5:
                saveGame("save.txt");
                logger.log("Game saved");
                break;
            case 6:
                loadGame("save.txt");
                logger.log("Game loaded");
                break;
            case 7:
                logger.log("Game exited");
                return;
            default:
                throw std::invalid_argument("Invalid choice");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            logger.log("Error: " + std::string(e.what()));
        }
    }
}

void Game::battle()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, monsters.size() - 1);

    auto &monster = *monsters[dis(gen)]; // Выбираем случайного монстра
    std::cout << "A wild " << monster.getName() << " appears!" << std::endl;
    logger.log("Battle started with " + monster.getName());

    while (player.getHp() > 0 && monster.getHp() > 0)
    {
        player.attackTarget(monster);
        logger.log(player.getName() + " attacked " + monster.getName());
        if (monster.getHp() <= 0)
        {
            std::cout << monster.getName() << " defeated!" << std::endl;
            player.gainExp(50);
            inventory.addItem("Monster Loot");
            logger.log(monster.getName() + " defeated, gained 50 EXP");
            break;
        }
        monster.attackTarget(player);
        logger.log(monster.getName() + " attacked " + player.getName());
        if (player.getHp() <= 0)
        {
            std::cout << "Game Over!" << std::endl;
            logger.log("Game Over: player died.");
            return;
        }
    }
}

void Game::saveGame(const std::string &filename) const
{
    std::ofstream out(filename);
    if (!out)
    {
        throw std::runtime_error("Cannot open save file");
    }
    player.save(out);
    inventory.save(out);
    out.close();
}

void Game::loadGame(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in)
    {
        throw std::runtime_error("Cannot open load file");
    }
    player.load(in);
    inventory.load(in);
    in.close();
}