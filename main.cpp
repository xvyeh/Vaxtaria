#include <iostream>
#include <string>

using namespace std;

enum class EquipmentType {
    WEAPON,
    ARMOR,
    OFFHAND,
    HELMET,
    BOOTS
};

const string BLACK = "\033[30m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string MAGENTA = "\033[35m";
const string ANSI_COLOR_RESET = "\033[0m";

struct Stats {
    int hp;
    int damage;
    int defense;
    Stats(int hp = 100, int damage = 10, int defense = 0) : hp(hp), damage(damage), defense(defense) {}
};

struct Item {
    string name;
    string rarity;
    int level;
    EquipmentType type;
    Stats stats;
    Item(const string& name, const string& rarity, EquipmentType type, Stats stats) : name(name), rarity(rarity), level(1), type(type), stats(stats) {}
};

class Inventory {
    Item*** grid;
    int rows;
    int cols;

public:
    Inventory(int rows = 5, int cols = 5) : rows(rows), cols(cols) {
        grid = new Item**[rows];
        for (int i = 0; i < rows; ++i) {
            grid[i] = new Item*[cols];
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = nullptr;
            }
        }
    }

    ~Inventory() {
        for (int i = 0; i < rows; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
    }

    Inventory(const Inventory&) = delete;

    Inventory& operator=(const Inventory&) = delete;

    bool remove(int r, int c) {
        if (grid[r][c] == nullptr) {
            return false;
        }

        grid[r][c] = nullptr;
        return true;
    }

    bool move(int r1, int c1, int r2, int c2) {
        if (grid[r1][c1] == nullptr) {
            cout << "Error: No item found at source position (" << r1 << ", " << c1 << ")" << endl;
            return false;
        }

        cout << "Moving item from (" << r1 << ", " << c1 << ") to (" << r2 << ", " << c2 << ")" << endl;

        grid[r2][c2] = grid[r1][c1];
        grid[r1][c1] = nullptr;
        return true;
    }

    bool levelUp(int r, int c) {
        if (grid[r][c] == nullptr) {
            return false;
        }

        grid[r][c]->level++;
        return true;
    }

    Item* getItem(int row, int col) const {
        if (row < rows && col < cols)
            return grid[row][col];
        else
            return nullptr;
    }

    bool add(Item* item) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == nullptr) {
                    grid[i][j] = item;
                    return true;
                }
            }
        }
        return false;
    }

    bool deleteItem(const string& itemName) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] != nullptr && grid[i][j]->name == itemName) {
                    grid[i][j] = nullptr;
                    return true;
                }
            }
        }
        return false;
    }

    void fillInventory() {
        add(new Item("Sword", "Common", EquipmentType::WEAPON, {0, 25, 0}));
        add(new Item("DArmor", "Epic", EquipmentType::ARMOR, {25, 0, 25}));
        add(new Item("DHelmet", "Epic", EquipmentType::HELMET, {15, 0, 15}));
        add(new Item("Boots", "Rare", EquipmentType::BOOTS, {10, 0, 15}));
        add(new Item("Potion", "Common", EquipmentType::OFFHAND, {10, 0, 0}));
        add(new Item("Torch", "Common", EquipmentType::OFFHAND, {0, 5, 0}));
        add(new Item("Shield", "Rare", EquipmentType::OFFHAND, {0, 0, 15}));
        add(new Item("DTooth", "Mythic", EquipmentType::OFFHAND, {100, 100, 100}));
        add(new Item("Oak Bow", "Rare", EquipmentType::WEAPON, {0, 35, 0}));
        add(new Item("DSpear", "Epic", EquipmentType::WEAPON, {0, 75, 0}));
        add(new Item("SpecY", "Spec", EquipmentType::OFFHAND, {999, 999, 999}));
    }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }
};

class Player {
    Item* equipment[5];
    Stats totalStats;
    Inventory inventory;
    int gold;
    int level;

public:
    Player() : totalStats({100, 10, 0}), inventory(5, 5), gold(1000) {
        for (int i = 0; i < 5; ++i) {
            equipment[i] = nullptr;
        }
    }

    bool equip(int row, int col) {
        Item* item = inventory.getItem(row, col);
        if (item == nullptr) {
            cout << "No item found at the specified position." << endl;
            return false;
        }
        EquipmentType type = item->type;
        if (equipment[static_cast<int>(type)] != nullptr) {
            cout << "Equipment slot already occupied." << endl;
            return false;
        }
        if (inventory.remove(row, col)) {
            equipment[static_cast<int>(type)] = item;
            totalStats.hp += item->stats.hp;
            totalStats.damage += item->stats.damage;
            totalStats.defense += item->stats.defense;
            return true;
        } else {
            cout << "Failed to remove item from inventory." << endl;
            return false;
        }
    }

    bool unequip(EquipmentType type) {
        if (equipment[static_cast<int>(type)] == nullptr) {
            return false;
        }
        inventory.add(equipment[static_cast<int>(type)]);
        totalStats.hp -= equipment[static_cast<int>(type)]->stats.hp;
        totalStats.damage -= equipment[static_cast<int>(type)]->stats.damage;
        totalStats.defense -= equipment[static_cast<int>(type)]->stats.defense;
        equipment[static_cast<int>(type)] = nullptr;
        return true;
    }

    void practice(){
        level++;
        cout << "Congratulations u are now: " << level << endl;
        totalStats.hp+= 10;
        totalStats.damage+=5;
        totalStats.defense+=2;
    }

    Stats getTotalStats() const {
        return totalStats;
    }

    Item* getEquipment(EquipmentType type) const {
        return equipment[static_cast<int>(type)];
    }

    Inventory* getInventory() {
        return &inventory;
    }

    int getGold() const {
        return gold;
    }

    int getLvl() const{
        return level;
    }
};

class DisplayConsole {
public:
    void displayInventory(const Inventory* inv) {
        cout << "Inventory:\n";
        for (int i = 0; i < inv->getRows(); i++) {
            for (int j = 0; j < inv->getCols(); j++) {
                if (inv->getItem(i, j) == nullptr) {
                    cout << "|------| ";
                } else {
                    const Item* item = inv->getItem(i, j);
                    string itemName = item->name;
                    if (item->rarity == "Rare") {
                        itemName = YELLOW + itemName + ANSI_COLOR_RESET;
                    } else if (item->rarity == "Epic") {
                        itemName = RED+ itemName + ANSI_COLOR_RESET;
                    } else if (item->rarity == "Common") {
                        itemName = GREEN + itemName + ANSI_COLOR_RESET;
                    } else if (item->rarity == "Mythic") {
                        itemName = MAGENTA + itemName + ANSI_COLOR_RESET;
                    } else if (item->rarity == "Spec") {
                        itemName = BLACK + itemName + ANSI_COLOR_RESET;
                }
                    cout << "|" << itemName << "| ";
                }
            }
            cout << endl;
        }
    }

    void displayEquipment(const Player* player) {
        cout << "Equipment:\n";
        for (int i = 0; i < 5; ++i) {
            string typeName;
            switch(i) {
            case 0:
                typeName = "Weapon";
                break;
            case 1:
                typeName = "Armor";
                break;
            case 2:
                typeName = "Offhand";
                break;
            case 3:
                typeName = "Helmet";
                break;
            case 4:
                typeName = "Boots";
                break;
            }
            if (player->getEquipment(static_cast<EquipmentType>(i)) != nullptr) {
                cout << typeName << ": " << player->getEquipment(static_cast<EquipmentType>(i))->name
                     << " (HP: " << player->getEquipment(static_cast<EquipmentType>(i))->stats.hp
                     << ", Damage: " << player->getEquipment(static_cast<EquipmentType>(i))->stats.damage
                     << ", Defense: " << player->getEquipment(static_cast<EquipmentType>(i))->stats.defense << ")" << endl;
            } else {
                cout << typeName << ": " << endl;
            }
        }
        cout << "Total Stats - HP: " << player->getTotalStats().hp
             << ", Damage: " << player->getTotalStats().damage
             << ", Defense: " << player->getTotalStats().defense << endl;

        cout << "Level: " << player->getLvl() << endl;

        cout << "Gold: " << player->getGold() << endl;
    }

    void displayMenu() {
        cout << "Menu:\n";
        cout << "1. Equip an item\n";
        cout << "2. Unequip an item\n";
        cout << "3. View inventory\n";
        cout << "4. View equipment\n";
        cout << "5. Exit\n";
        cout << "6. Practice\n";
    }
};

int main() {
    Player P;
    P.getInventory()->fillInventory();
    DisplayConsole D;
    int choice;
    do {
        cout << string(50, '\n');

        D.displayInventory(P.getInventory());
        D.displayEquipment(&P);
        D.displayMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int row, col;
            cout << "Enter the row and column to equip the item: ";
            cin >> row >> col;
            if (P.equip(row, col)) {
                cout << "Item equipped successfully." << endl;
            } else {
                cout << "Failed to equip item." << endl;
            }
            break;
        }
        case 2: {
            int slot;
            cout << "Enter the slot number to unequip the item (0-4): ";
            cin >> slot;
            if (P.unequip(static_cast<EquipmentType>(slot))) {
                cout << "Item unequipped successfully." << endl;
            } else {
                cout << "Failed to unequip item." << endl;
            }
            break;
        }
        case 3:
            D.displayInventory(P.getInventory());
            break;
        case 4:
            D.displayEquipment(&P);
            break;
        case 5:
            cout << "Exiting program." << endl;
            break;
        case 6:
            P.practice();
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 5);

    return 0;
}
