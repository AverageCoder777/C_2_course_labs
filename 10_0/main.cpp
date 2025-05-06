#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// Базовый класс User
class User
{
protected:
    std::string name_;
    int id_;
    int accessLevel_;

public:
    User(const std::string &name, int id, int accessLevel)
        : name_(name), id_(id), accessLevel_(accessLevel)
    {
        if (name.empty())
            throw std::invalid_argument("Name cannot be empty");
        if (id < 0)
            throw std::invalid_argument("ID cannot be negative");
        if (accessLevel < 0)
            throw std::invalid_argument("Access level cannot be negative");
    }

    virtual ~User() = default;

    // Геттеры и сеттеры
    std::string getName() const { return name_; }
    int getId() const { return id_; }
    int getAccessLevel() const { return accessLevel_; }

    void setName(const std::string &name)
    {
        if (name.empty())
            throw std::invalid_argument("Name cannot be empty");
        name_ = name;
    }

    virtual void displayInfo() const = 0;

    virtual void serialize(std::ofstream &ofs) const
    {
        ofs << name_ << '\n'
            << id_ << '\n'
            << accessLevel_ << '\n';
    }

    virtual void deserialize(std::ifstream &ifs)
    {
        std::getline(ifs, name_);
        if (name_.empty() && !ifs.eof())
        {
            throw std::runtime_error("Failed to read name from file");
        }
        ifs >> id_ >> accessLevel_;
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка до конца строки
    }
};

// Производные классы
class Student : public User
{
private:
    std::string group_;

public:
    Student(const std::string &name, int id, int accessLevel, const std::string &group)
        : User(name, id, accessLevel), group_(group)
    {
        if (group.empty())
            throw std::invalid_argument("Group cannot be empty");
    }

    void displayInfo() const override
    {
        std::cout << "Student: " << name_ << ", ID: " << id_
                  << ", Access Level: " << accessLevel_ << ", Group: " << group_ << std::endl;
    }

    void serialize(std::ofstream &ofs) const override
    {
        ofs << "Student\n";
        User::serialize(ofs);
        ofs << group_ << '\n';
    }

    void deserialize(std::ifstream &ifs) override
    {
        User::deserialize(ifs);
        std::getline(ifs, group_);
        if (group_.empty() && !ifs.eof())
        {
            throw std::runtime_error("Failed to read group from file");
        }
    }
};

class Teacher : public User
{
private:
    std::string department_;

public:
    Teacher(const std::string &name, int id, int accessLevel, const std::string &department)
        : User(name, id, accessLevel), department_(department)
    {
        if (department.empty())
            throw std::invalid_argument("Department cannot be empty");
    }

    void displayInfo() const override
    {
        std::cout << "Teacher: " << name_ << ", ID: " << id_
                  << ", Access Level: " << accessLevel_ << ", Department: " << department_ << std::endl;
    }

    void serialize(std::ofstream &ofs) const override
    {
        ofs << "Teacher\n";
        User::serialize(ofs);
        ofs << department_ << '\n';
    }

    void deserialize(std::ifstream &ifs) override
    {
        User::deserialize(ifs);
        std::getline(ifs, department_);
        if (department_.empty() && !ifs.eof())
        {
            throw std::runtime_error("Failed to read department from file");
        }
    }
};

class Administrator : public User
{
private:
    std::string role_;

public:
    Administrator(const std::string &name, int id, int accessLevel, const std::string &role)
        : User(name, id, accessLevel), role_(role)
    {
        if (role.empty())
            throw std::invalid_argument("Role cannot be empty");
    }

    void displayInfo() const override
    {
        std::cout << "Administrator: " << name_ << ", ID: " << id_
                  << ", Access Level: " << accessLevel_ << ", Role: " << role_ << std::endl;
    }

    void serialize(std::ofstream &ofs) const override
    {
        ofs << "Administrator\n";
        User::serialize(ofs);
        ofs << role_ << '\n';
    }

    void deserialize(std::ifstream &ifs) override
    {
        User::deserialize(ifs);
        std::getline(ifs, role_);
        if (role_.empty() && !ifs.eof())
        {
            throw std::runtime_error("Failed to read role from file");
        }
    }
};

// Класс Resource
class Resource
{
private:
    std::string name_;
    int requiredAccessLevel_;

public:
    Resource(const std::string &name, int requiredAccessLevel)
        : name_(name), requiredAccessLevel_(requiredAccessLevel)
    {
        if (name.empty())
            throw std::invalid_argument("Resource name cannot be empty");
        if (requiredAccessLevel < 0)
            throw std::invalid_argument("Required access level cannot be negative");
    }

    bool checkAccess(const User &user) const
    {
        return user.getAccessLevel() >= requiredAccessLevel_;
    }

    std::string getName() const { return name_; }
    int getRequiredAccessLevel() const { return requiredAccessLevel_; }

    void serialize(std::ofstream &ofs) const
    {
        ofs << name_ << '\n'
            << requiredAccessLevel_ << '\n';
    }

    void deserialize(std::ifstream &ifs)
    {
        std::getline(ifs, name_);
        if (name_.empty() && !ifs.eof())
        {
            throw std::runtime_error("Failed to read resource name from file");
        }
        ifs >> requiredAccessLevel_;
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

// Шаблонный класс AccessControlSystem
template <typename T>
class AccessControlSystem
{
private:
    std::vector<std::unique_ptr<User>> users_;
    std::vector<Resource> resources_;

public:
    void addUser(std::unique_ptr<User> user)
    {
        users_.push_back(std::move(user));
    }

    void addResource(const Resource &resource)
    {
        resources_.push_back(resource);
    }

    bool checkAccess(const User &user, const std::string &resourceName) const
    {
        auto it = std::find_if(resources_.begin(), resources_.end(),
                               [&resourceName](const Resource &r)
                               { return r.getName() == resourceName; });

        if (it == resources_.end())
        {
            throw std::invalid_argument("Resource not found");
        }

        return it->checkAccess(user);
    }

    void displayAllUsers() const
    {
        if (users_.empty())
        {
            std::cout << "No users in the system.\n";
            return;
        }
        for (const auto &user : users_)
        {
            user->displayInfo();
        }
    }

    User *findUserByName(const std::string &name) const
    {
        auto it = std::find_if(users_.begin(), users_.end(),
                               [&name](const auto &user)
                               { return user->getName() == name; });

        return (it != users_.end()) ? it->get() : nullptr;
    }

    User *findUserById(int id) const
    {
        auto it = std::find_if(users_.begin(), users_.end(),
                               [id](const auto &user)
                               { return user->getId() == id; });

        return (it != users_.end()) ? it->get() : nullptr;
    }

    void sortUsersByAccessLevel()
    {
        std::sort(users_.begin(), users_.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a->getAccessLevel() < b->getAccessLevel();
                  });
    }

    void sortUsersByName()
    {
        std::sort(users_.begin(), users_.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a->getName() < b->getName();
                  });
    }

    void sortUsersById()
    {
        std::sort(users_.begin(), users_.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a->getId() < b->getId();
                  });
    }

    void saveToFile(const std::string &filename) const
    {
        std::ofstream ofs(filename);
        if (!ofs)
            throw std::runtime_error("Cannot open file for writing");

        ofs << users_.size() << '\n';
        for (const auto &user : users_)
        {
            user->serialize(ofs);
        }

        ofs << resources_.size() << '\n';
        for (const auto &resource : resources_)
        {
            resource.serialize(ofs);
        }
        ofs.close();
    }

    void loadFromFile(const std::string &filename)
    {
        std::ifstream ifs(filename);
        if (!ifs)
            throw std::runtime_error("Cannot open file for reading");

        users_.clear();
        resources_.clear();

        size_t userCount;
        ifs >> userCount;
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (size_t i = 0; i < userCount && ifs.good(); ++i)
        {
            std::string type;
            std::getline(ifs, type);
            if (type.empty() && !ifs.eof())
            {
                throw std::runtime_error("Failed to read user type from file");
            }

            std::unique_ptr<User> user;
            if (type == "Student")
            {
                user = std::make_unique<Student>("temp", 0, 0, "temp");
            }
            else if (type == "Teacher")
            {
                user = std::make_unique<Teacher>("temp", 0, 0, "temp");
            }
            else if (type == "Administrator")
            {
                user = std::make_unique<Administrator>("temp", 0, 0, "temp");
            }
            else
            {
                throw std::runtime_error("Unknown user type: " + type);
            }

            if (user)
            {
                user->deserialize(ifs);
                users_.push_back(std::move(user));
            }
        }

        size_t resourceCount;
        ifs >> resourceCount;
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (size_t i = 0; i < resourceCount && ifs.good(); ++i)
        {
            Resource resource("temp", 0);
            resource.deserialize(ifs);
            resources_.push_back(resource);
        }
        ifs.close();
    }
};
void displayMenu()
{
    std::cout << "\nMenu:\n"
              << "1. Add User\n"
              << "2. Find User by Name\n"
              << "3. Find User by ID\n"
              << "4. Sort Users by Access Level\n"
              << "5. Sort Users by Name\n"
              << "6. Sort Users by ID\n"
              << "7. Display All Users\n"
              << "8. Save Data to File\n"
              << "9. Load Data from File\n"
              << "10. Exit\n"
              << "Enter your choice: ";
}

void addUser(AccessControlSystem<int> &system)
{
    std::cout << "Enter user type (Student/Teacher/Administrator): ";
    std::string type;
    std::cin >> type;

    std::cout << "Enter name: ";
    std::string name;
    std::cin.ignore();
    std::getline(std::cin, name);

    std::cout << "Enter ID: ";
    int id;
    std::cin >> id;

    std::cout << "Enter access level: ";
    int accessLevel;
    std::cin >> accessLevel;

    if (type == "Student")
    {
        std::cout << "Enter group: ";
        std::string group;
        std::cin.ignore();
        std::getline(std::cin, group);
        system.addUser(std::make_unique<Student>(name, id, accessLevel, group));
    }
    else if (type == "Teacher")
    {
        std::cout << "Enter department: ";
        std::string department;
        std::cin.ignore();
        std::getline(std::cin, department);
        system.addUser(std::make_unique<Teacher>(name, id, accessLevel, department));
    }
    else if (type == "Administrator")
    {
        std::cout << "Enter role: ";
        std::string role;
        std::cin.ignore();
        std::getline(std::cin, role);
        system.addUser(std::make_unique<Administrator>(name, id, accessLevel, role));
    }
    else
    {
        std::cout << "Invalid user type.\n";
    }
}

void findUserByName(const AccessControlSystem<int> &system)
{
    std::cout << "Enter name: ";
    std::string name;
    std::cin.ignore();
    std::getline(std::cin, name);

    User *user = system.findUserByName(name);
    if (user)
    {
        user->displayInfo();
    }
    else
    {
        std::cout << "User not found.\n";
    }
}

void findUserById(const AccessControlSystem<int> &system)
{
    std::cout << "Enter ID: ";
    int id;
    std::cin >> id;

    User *user = system.findUserById(id);
    if (user)
    {
        user->displayInfo();
    }
    else
    {
        std::cout << "User not found.\n";
    }
}

void saveData(const AccessControlSystem<int> &system)
{
    std::cout << "Enter filename to save data: ";
    std::string filename;
    std::cin.ignore();
    std::getline(std::cin, filename);

    try
    {
        system.saveToFile(filename);
        std::cout << "Data saved successfully to " << filename << ".\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error saving data: " << e.what() << std::endl;
    }
}

void loadData(AccessControlSystem<int> &system)
{
    std::cout << "Enter filename to load data: ";
    std::string filename;
    std::cin.ignore();
    std::getline(std::cin, filename);

    try
    {
        system.loadFromFile(filename);
        std::cout << "Data loaded successfully from " << filename << ".\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading data: " << e.what() << std::endl;
    }
}

// Основная программа
int main() {
    try {
        AccessControlSystem<int> system;

        int choice;
        do {
            displayMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    addUser(system);
                    break;
                case 2:
                    findUserByName(system);
                    break;
                case 3:
                    findUserById(system);
                    break;
                case 4:
                    system.sortUsersByAccessLevel();
                    std::cout << "Users sorted by access level.\n";
                    break;
                case 5:
                    system.sortUsersByName();
                    std::cout << "Users sorted by name.\n";
                    break;
                case 6:
                    system.sortUsersById();
                    std::cout << "Users sorted by ID.\n";
                    break;
                case 7:
                    system.displayAllUsers();
                    break;
                case 8:
                    saveData(system);
                    break;
                case 9:
                    loadData(system);
                    break;
                case 10:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 10);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}