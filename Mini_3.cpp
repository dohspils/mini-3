#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

//weapon armor consumable
//============ ITEM CLASS
enum class ItemType {
	WEAPON,
	ARMOR,
	CONSUMABLE
};

class Item {
private:
	string name{};
	int weight{};
	ItemType type{};

public:

	virtual void displayItemInfo() = 0;
	virtual int getValue() = 0;
	Item(string name, int weight, ItemType type) 
		: name(name), weight(weight), type(type) {}
	string getName() { return name; }
	int getWeight() { return weight; }
	ItemType getType() { return type; }
};

//============ WEAPON CLASS
class Weapon : public Item {
private:
	int damage{};
public:
	Weapon(string name, int weight, int damage) 
		: Item(name, weight, ItemType::WEAPON), damage(damage){}
	void displayItemInfo() override {
		cout << "[" << getName() << "]" << " - (weight): " << getWeight() << " (damage): " << damage;
	}

	int getValue() override {
		return damage;
	}
};

//============ ARMOR CLASS
class Armor : public Item {
private:
	int defense{};
public:
	Armor(string name, int weight, int defense)
		: Item(name, weight, ItemType::ARMOR), defense(defense) {}

	void displayItemInfo() override {
		cout << "[" << getName() << "]" << " - (weight): " << getWeight() << " (defense): " << defense;
	}
	int getValue() override {
		return defense;
	}

};


//============ CONSUMABLE CLASS
class Consumable : public Item {
private:
	int value{};
public:
	Consumable(string name, int weight, int value)
		: Item(name, weight, ItemType::CONSUMABLE), value(value) {}

	void displayItemInfo() override {
		cout << "[" << getName() << "]" << " - (weight): " << getWeight() << " (healing): " << value;
	}

	int getValue() override {
		return value;
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
	// TYPE TO STRING / STRING TO TYPE
	string typeToString(ItemType itemType) {
		switch (itemType) {
		case ItemType::WEAPON:
			return "weapon";
		case ItemType::ARMOR:
			return "armor";
		case ItemType::CONSUMABLE:
			return "consumable";
		default:
			break;
		}
	}

	static ItemType stringToType(string itemType) {
		if (itemType == "weapon") { return ItemType::WEAPON; }
		if (itemType == "armor") { return ItemType::ARMOR; }
		if (itemType == "consumable") { return ItemType::CONSUMABLE; }
	}

	void savePlayer(const string& fileName) {
		json data;

		data["playerName"] = name;
		data["health"] = health;


		data["inventory"] = json::array();

		for (auto& i : playerInventory->getStorage()) {
			data["inventory"].push_back({{"name", i->getName()},
				{"weight", i->getWeight()}, {"value", i->getValue()}, {"type", typeToString(i->getType())} });
		}
		 
		ofstream file(fileName);
		file << data.dump(4);
	}
	
	static optional<unique_ptr<Player>> loadPlayer(const string& fileName) {
		if (filesystem::exists(fileName)) {
			ifstream file(fileName);
			json data = json::parse(file);

			unique_ptr<Player> loadedPlayer = make_unique<Player>(data["playerName"], data["health"]);

			for (auto item : data["inventory"]) {
				ItemType itemEnum = stringToType(item["type"]);

				switch (itemEnum) {
				case ItemType::WEAPON:
					loadedPlayer->addItem(move(make_unique<Weapon>(item["name"], item["weight"], item["value"])));
					break;
				case ItemType::ARMOR:
					loadedPlayer->addItem(move(make_unique<Armor>(item["name"], item["weight"], item["value"])));
					break;
				case ItemType::CONSUMABLE:
					loadedPlayer->addItem(move(make_unique<Consumable>(item["name"], item["weight"], item["value"])));
					break;
				default:
					break;
				}
			}	
			return move(loadedPlayer);
		}
		else { return nullopt; }
		
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
				savePlayer(name + ".json");
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

	bool playerCreate(){
		string inputName{};
		cout << "Enter name: ";
		cin >> inputName;
		if (filesystem::exists(inputName + ".json")) {
			cout << "A player under than name is already created. Try loading!\n";
			return false;
		}
		int inputHealth{};
		cout << "Enter health: ";
		
		while (!(cin >> inputHealth)) {
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Enter a valid number for health: ";
		}
		createPlayer(inputName, inputHealth);
		return true;
	}

	bool loadPlayerScreen() {
		string inputName{};
		cout << "Enter the name of the player you want to load: ";
		cin >> inputName;
		cout << "\n";

		auto loadedPlayer = Player::loadPlayer(inputName + ".json");
		if (loadedPlayer.has_value()) {
			currentPlayer = move(loadedPlayer.value());
			return true;
		}
		else {
			cout << "That player does not exist.\n";
			return false;
		}
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
				if (!playerCreate()) {
					continue;
				}
				break;
			case '2':
				if (loadPlayerScreen()) {
					cout << "Player Found!\n";
				}
				else {
					continue;
				}
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