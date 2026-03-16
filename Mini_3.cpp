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
	
	void addItem() {
	
	}


};

class Player {
private:
	string name{};
	int health{};
	unique_ptr<Inventory> playerInventory;
public:
	Inventory* getPlayerInventory() { return playerInventory.get(); }
	string getName() { return name; }
	Player(string name = "default_player", int health = 100)
		: name(name), health(health) 
	{
		playerInventory = make_unique<Inventory>(this, 15);
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
			cout << p->getPlayerInventory()->getOwner().getName();
		}
	}
};

int main() {
	World world;
	world.createPlayer("slipshod", 100);
	world.displayPlayers();

	return 0;
}