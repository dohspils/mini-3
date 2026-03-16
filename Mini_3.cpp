#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

//weapon armor consumable
//============ ITEM CLASS
class Item {
private:
	string name{};
	int weight{};

public:
	Item(string name, int weight) 
		: name(name), weight(weight) {}

};
//============ WEAPON CLASS
class Weapon : public Item {
private:
	int damage{};
public:
	Weapon(string name, int weight, int damage) 
		: Item(name, weight), damage(damage){}

};

//============ ARMOR CLASS
class Armor : public Item {
private:
	int defense{};
public:
	Armor(string name, int weight, int defense)
		: Item(name, weight), defense(defense) {}

};


//============ CONSUMABLE CLASS
class Consumable : public Item {
private:
	int value{};
public:
	Consumable(string name, int weight, int value)
		: Item(name, weight), value(value) {}
	
};

/*An Inventory class that holds a collection of items with a max weight capacity. Supports adding, removing, and listing items. If adding an item would exceed the weight limit, reject it.*/

class Player; //forward declaration

class Inventory {
private:
	Player* owner{nullptr};
	vector<unique_ptr<Item>> storage{};
	int max_weight{};

public:

	Player& getOwner() { return *owner; }

	Inventory() = default;
	Inventory(Player* owner = nullptr, int max_weight = 10) 
		: owner(owner), max_weight(max_weight) {}
	
	void addItem(unique_ptr<Item>& item) {
	
	}

	


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
		playerInventory = make_unique<Inventory>(this, 15);
	}
	Inventory* getPlayerInventory() { return playerInventory.get(); }
	string getName() { return name; }
	int getHealth() { return health; }
	void setHealth(int h) { health = h; }


	void addItem(unique_ptr<Item>& item) {
		playerInventory->addItem(item);
	}

	

};

class World {
private:
	vector<unique_ptr<Player>> Players{};

public:
	World() = default;
	
	void createPlayer(string name, int health) {
		unique_ptr<Player> newPlayer = make_unique<Player>(name, health);
		Players.push_back(move(newPlayer));
	}

	void displayPlayers() {
		for (auto& p : Players) {
			cout << p->getHealth();
		}
	}

	Player* getPlayer(string name) {
		for (auto& p : Players) {
			if (p->getName() == name) {
				return p.get();
			}
			else {
				nullptr;
			}
		}
	}

	void worldMain() {
		createPlayer("slipshod", 100);
		//displayPlayers();
		Player* slipshod = getPlayer("slipshod");
		cout << slipshod->getName();
		slipshod->setHealth(5);
		displayPlayers();
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