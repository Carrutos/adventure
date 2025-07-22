
#include <vector>
#include <iostream>

struct door {
	std::string name;
	int target;
};

	struct room {
		std::string name;
		std::string desc;
		std::vector<door>doors;
		std::vector<std::string>items;
	};

int main()
{
	enum{sweet, pelmeni, carrot, apple, cucumber, water, milk, jam, noodles, beer, key};
	std::string itemname[11] = {"sweet", "pelmeni", "carrot", "apple", "cucumber", "water", "milk", "jam", "noodles", "beer", "key"};
	room list[13];
	list[0] = { "oven","cooks food" };
	list[1] = { "stove","also cooks food, but differently" };
	list[2] = { "fridge","stores food" };
	list[3] = { "closet","also stores food" };
	list[4] = { "kitchenHall","leads to kitchen furniture and other places" };
	list[5] = { "hall","leads to the exit and contains a key" };
	list[6] = { "exit","ends the game" };
	list[7] = { "storageRoom","leads to other rooms" };
	list[8] = { "pool","big water reserve" };
	list[9] = { "closet1","stores food, the one in storage" };
	list[10] = { "closet2","stores food, the one in storage" };
	list[11] = { "iceRoom","freezes everything" };
	list[12] = { "man","eats food" };
	list[0].doors = { { "oven-stove",1 },{ "oven-fridge",2 },{ "oven-kitchenHall",4 } };
	list[1].doors = { { "stove-oven",0 },{ "stove-closet",3 },{ "stove-kitchenHall",4 } };
	list[2].items = { itemname[4], itemname[3], itemname[6] };
	int choice = 0;
	int hp = 51;
	std::vector<std::string>myItems = {};
	std::string command;
	while (command != "out") {
	std::cout << "you are in " << list[choice].name << std::endl;
	std::cin >> command;
	if (command == "go") {
		std::cout << "where do you wanna go" << std::endl;
		for (int i = 0; i < list[choice].doors.size(); i++) {
			std::cout << list[choice].doors[i].target << " - " << list[list[choice].doors[i].target].name << std::endl;
		}
		std::cin >> choice;
	}
	else if (command == "list") {
		if (list[choice].items.empty()) {
			std::cout << "empty" << std::endl;
		}
		else {
			for (int i = 0; i < list[choice].items.size(); i++) {
				std::cout << list[choice].items[i] << std::endl;
			}
		}
	}
	else if (command == "trunk") {
		if (myItems.empty()) {
			std::cout << "empty" << std::endl;
		}
		else {
			for (int i = 0; i < myItems.size(); i++) {
				std::cout << myItems[i] << std::endl;
			}
		}
	}
	else if (command == "pick") {
		std::string pickItem;
		std::cout << "which item?" << std::endl;
		std::cin >> pickItem;
		if (list[choice].items.empty()) {
			std::cout << "empty" << std::endl;
		}
		else {
			for (int i = 0; i < 10; i++) {
				if (pickItem == list[choice].items[i]) {
					myItems.push_back(list[choice].items[i]);
					list[choice].items.erase(list[choice].items.cbegin()+i);
					std::cout << "plus odin" << std::endl;
				}
			}
		}
	}
	else if (command == "drop") {
		std::string dropItem;
		std::cout << "which item?" << std::endl;
		std::cin >> dropItem;
		if (myItems.empty()) {
			std::cout << "empty" << std::endl;
		}
		else {
			for (int i = 0; i < 10; i++) {
				if (dropItem == myItems[i]) {
					list[choice].items.push_back(myItems[i]);
					myItems.erase(myItems.cbegin() + i);
					std::cout << "minus odin" << std::endl;
				}
			}
		}
	}
	else if (command == "use") {
		std::string useItem;
		if (myItems.empty()) {
			std::cout << "nothing" << std::endl;
		}
		else {
			std::cout << "choose an item" << std::endl;
			std::cin >> useItem;
		}
	}
	}

}



