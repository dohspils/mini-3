#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

//weapon armor consumable
//============ ITEM CLASS
class Item {
private:
	string name{};
	int weight{};

public:

	virtual void displayItemInfo() = 0;
	Item(string name, int weight) 
		: name(name), weight(weight) {}
	string getName() { return name; }
	int getWeight() { return weight; }
};

//============ WEAPON CLASS
class Weapon : public Item {
private:
	int damage{};
public:
	Weapon(string name, int weight, int damage) 
		: Item(name, weight), damage(damage){}
	void displayItemInfo() override {
		cout << "[" << getName() << "]" << " - (weight): " << getWeight() << " (damage): " << damage;
	}
};

//============ ARMOR CLASS
class Armor : public Item {
private:
	int defense{};
public:
	Armor(string name, int weight, int defense)
		: Item(name, weight), defense(defense) {}

	void displayItemInfo() override {
		cout << "[" << getName() << "]" << " - (weight): " << getWeight() << " (defense): " << defense;
	}
};


//============ CONSUMABLE CLASS
class Consumable : public Item {
private:
	int value{};
public:
	Consumable(string name, int weight, int value)
		: Item(name, weight), value(value) {}

	void displayItemInfo() override {
		cout << "[" << getName() << "]" << " - (weight): " << getWeight() << " (healing): " << value;
	}
};

/*An Inventory class that holds a collection of items with a max weight capacity. Supports adding, removing, and listing items. If adding an item would exceed the weight limit, reject it.*/

class Player; //forward declaration

class Inventory {
private:
	Player* owner{nullptr};
	vector<unique_ptr<Item>> storage{};
	int currentWeight{0};
	int maxWeight{};

public:
	vector<unique_ptr<Item>>& getStorage() { return storage; }
	Player& getOwner() { return *owner; }

	Inventory() = default;
	Inventory(Player* owner = nullptr, int maxWeight = 30) 
		: owner(owner), maxWeight(maxWeight) {}

	//== Function definitions are below player ==
	void indexInventory();
	void addItem(unique_ptr<Item>);
	void removeItem(int i);
};

class Player {
private:
	string name{};
	int health{};
	unique_ptr<Inventory> playerInventory;
public:
	Player(string name = "default_player", int health = 100)
		: name(name), health(health) 
	{
		playerInventory = make_unique<Inventory>(this, 100);
	}
	Inventory* getPlayerInventory() { return playerInventory.get(); }
	string getName() { return name; }
	int getHealth() { return health; }
	void setHealth(int h) { health = h; }


	void addItem(unique_ptr<Item> item) {
		playerInventory->addItem(move(item));
	}
	void removeItemMenu() {
		if (playerInventory->getStorage().empty()) { cout << "[!] inventory is empty [!]\n";  return; }
		playerInventory->indexInventory();
		int response{};
		cout << "Type the number correlated to the item to remove.\n";
		while (!(cin >> response)) {
			cin.clear();
			cin.ignore(100, '\n');
			cout << "invalid, try again.\n";
		}
		playerInventory->removeItem(response - 1);
	}


	void mainMenu() {
		

		char response{};
		do {
			cout << "\n\n============ MAIN MENU ============\n\n";
			cout << "(1) - give weapon\n";
			cout << "(2) - give armor\n";
			cout << "(3) - give consumable\n";
			cout << "___________________________________\n";
			cout << "(r) - remove item\n";
			cout << "(s) - save & quit\n";
			cout << "===================================\n";


			cin >> response;
			switch (response) {
			case '1':
				addItem(make_unique<Weapon>("Sword", 45, 20));
				break;
			case '2':
				addItem(make_unique<Armor>("Helmet", 45, 7));
				break;
			case '3':
				addItem(make_unique<Consumable>("Potion", 5, 20));
				break;
			case 'r':
				removeItemMenu();
				break;
			case 's':
				break;
			default:
				cout << "invalid, try again: ";
				break;
			}
		} while (response != 'S' && response != 's');
		cout << "SAVEEE!!!!!\n";
	}
};

//=========== EXTERNAL INVENTORY FUNCTIONS (REQUIRE PLAYER) =========== 

void Inventory::indexInventory() {
	if (storage.empty()) { return; }
	for (size_t i{0}; i < storage.size(); i++) {
		cout << "[" << i+1 << "]" << " = "; storage.at(i)->displayItemInfo(); cout << "\n";
	}
}

//add function
void Inventory::addItem(unique_ptr<Item> item) {
	if (currentWeight + item->getWeight() <= maxWeight) {
		cout << owner->getName() << " recieved : " << item->getName() << "\n";
		currentWeight += item->getWeight();
		storage.push_back(move(item)); // add item
	}
	else {
		cout << "item " << item->getName() << " is to heavy to pick up.\n";
	}
}
//remove function
void Inventory::removeItem(int i) {
	if (storage.empty()) { cout << "inventory is empty.\n"; return; }
	if (storage.at(i)) {
		string nameOfItem = storage.at(i)->getName();
		currentWeight -= storage.at(i)->getWeight();
		storage.erase(storage.begin() + i);
		cout << "item \"" << nameOfItem << "\" has been removed.\n";
	}
}
//======================================================================

class World {
private:
	unique_ptr<Player> currentPlayer{};

public:
	World() = default;
	
	void createPlayer(string name, int health) {
		unique_ptr<Player> newPlayer = make_unique<Player>(name, health);
		currentPlayer = (move(newPlayer));
	}

	void playerCreate(){
		string inputName{};
		cout << "Enter name: ";
		cin >> inputName;
		int inputHealth{};
		cout << "Enter health: ";
		
		while (!(cin >> inputHealth)) {
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Enter a valid number for health: ";
		}
		createPlayer(inputName, inputHealth);
	}

	void loadPlayer() {
	
	}

	void startScreen() {
		char response{};
		cout << "---===[ Mini Project 3 ]===---\n";
		while (true) {
			cout << "(1) - create player\n";
			cout << "(2) - load player\n";
			cout << ": ";
			cin >> response;
			cout << "\n";

			switch (response) {
			case '1':
				playerCreate();
				break;
			case '2':
				loadPlayer();
				break;
			default:
				cout << "please enter a valid option\n";
				continue;
			}
			break;
		}
	}

	void worldMain() {
		startScreen();
		currentPlayer->mainMenu();
	} 
};

int main() {
	World world;
	world.worldMain();
	
	return 0;
}

/*What to Build
1. Item System
A base Item class with derived types — at minimum Weapon, Armor, and Consumable. Each item has a name, weight, and value. Derived types add their own relevant stats (damage, defense, heal amount, etc.).
2. Inventory
An Inventory class that holds a collection of items with a max weight capacity. Supports adding, removing, and listing items. If adding an item would exceed the weight limit, reject it.
3. World Class
This is where you apply what we just talked about. A World class owns the player, the inventory, and anything else that exists. Main just creates World and calls run.
4. Save System
Serialize the inventory to a .txt file. Each item gets written as a line of data. Keep the format simple and human-readable:
Weapon,Sword,3.5,150,25
Armor,ChestPlate,8.0,200,15
Consumable,HealthPotion,0.5,50,30
5. Load System
Read the file back in, parse each line, reconstruct the correct derived item type, and repopulate the inventory.

Turn Loop
1. Print inventory contents and current weight
2. Player chooses action (add item, remove item, save, load, quit)
3. Execute action
4. Repeat

Constraints

World class owns everything
All heap objects use unique_ptr
Save file is plain .txt, human readable
No combat needed — pure inventory management


You've Nailed It If

Saving then closing the program then loading brings back the exact same inventory
Adding an item over the weight limit is rejected cleanly
Adding a new item type means writing one new class and updating the save/load parser, nothing else*/