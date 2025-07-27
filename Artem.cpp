
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

struct useResult {
	std::string start;
	std::string end;
};

struct combineResult {
	std::string item1;
	std::string item2;
	std::string result;
};

int main()
{
	//enum { chocolate, pelmeni, carrot, apple, tomato, water, milk, jam, noodles, sauce, fish, lemon, key };
	std::string itemname[11] = { "chocolate", "frozenPelmeni", "carrot", "apple", "tomato", "water", "milk", "jam", "dryNoodles", "sauce", "key" };
	int react[11] = { 4, 1, 3, 3, 2, 2, 3, 3, 2, 2, 1 };
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
	list[2].doors = { { "fridge-oven",0 },{ "fridge-kitchenHall",4 } };
	list[2].items = { itemname[4], itemname[3], itemname[6] };
	list[3].doors = { { "closet-stove",1 },{ "closet-kitchenHall",4 } };
	list[3].items = { itemname[3], itemname[8] };
	list[4].doors = { { "kitchenHall-oven",0 },{ "kitchenHall-stove",1 },{"kitchenHall-fridge",2},{"kitchenHall-closet",3},{"kitchenHall-hall",5},
		{"kitchenHall-storageRoom",7},{"kitchenHall-man",12}};
	list[4].items = { itemname[0] };
	list[5].doors = { { "hall-kitchenHall",4 },{ "hall-exit",6 } };
	list[5].items = { itemname[12] };
	list[6].doors = { { "exit-hall",5 } };
	list[7].doors = { { "storageRoom-kitchenHall",4 },{ "storageRoom-pool",8 },{"storageRoom-closet1",9},{"storageRoom-closet2",10},{"storageRoom-iceRoom",11} };
	list[7].items = { itemname[0] };
	list[8].doors = { { "pool-storageRoom",7 } };
	list[8].items = { itemname[5] };
	list[9].doors = { { "closet1-storageRoom",7 },{ "closet1-closet2",10}};
	list[9].items = { itemname[7], itemname[9] };
	list[10].doors = { { "closet2-storageRoom",7 },{ "closet2-closet1",9} };
	list[10].items = { itemname[2], itemname[11] };
	list[11].doors = { { "iceRoom-storageRoom",7 } };
	list[11].items = { itemname[1], itemname[10] };
	list[12].doors = { { "man-kitchenHall",4 } };
	int choice = 0;
	int hp = 41;
	std::vector<std::string>myItems = {};
	std::string command;
	int feed = 0;
	std::cout << "you wake up in the kitchen of some fat guy's house and are told to feed him something" << std::endl;
	while (choice != 13 || hp > 0 || feed < 50) {
		std::cout << "you are in " << list[choice].name << std::endl;
		std::cin >> command;
		if (command == "go") {
			std::cout << "where do you wanna go (pick a number)" << std::endl;
			for (int i = 0; i < list[choice].doors.size(); i++) {
				std::cout << list[choice].doors[i].target << " - " << list[list[choice].doors[i].target].name << std::endl;
			}
			int choiceOld = choice;
			std::cin >> choice;
			for (int i = 0; i < list[choiceOld].doors.size(); i++) {
				int counter = 0;
				if (choice == list[choiceOld].doors[i].target) {
					counter++;
				}
			}
			if (counter == 0) {
				choice == choiceOld;
				std::cout << "you cannot go there" << std::endl;
			}
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
			if (list[choice].items.empty()) {
				std::cout << "no items here" << std::endl;
			}
			else {
				std::cout << "which item?" << std::endl;
				std::cin >> pickItem;
				if (choice == 8 && pickItem == "water") {
					myItems.push_back(list[choice].items[i]);
					std::cout << "item picked successfully" << std::endl;
				}
				else {
					for (int i = 0; i < 10; i++) {
						if (pickItem == list[choice].items[i]) {
							myItems.push_back(list[choice].items[i]);
							list[choice].items.erase(list[choice].items.cbegin() + i);
							std::cout << "item picked successfully" << std::endl;
						}
					}
				}
			}
		}
		else if (command == "drop") {
			std::string dropItem;
			if (myItems.empty()) {
				std::cout << "you have no items" << std::endl;
			}
			else {
				std::cout << "which item?" << std::endl;
				std::cin >> dropItem;
				for (int i = 0; i < 10; i++) {
					if (dropItem == myItems[i]) {
						list[choice].items.push_back(myItems[i]);
						myItems.erase(myItems.cbegin() + i);
						std::cout << "item dropped successfully" << std::endl;
					}
				}
			}
		}
		else if (command == "use") {
			std::string useItem;
			if (myItems.empty()) {
				std::cout << "nothing to use" << std::endl;
			}
			else {
				std::cout << "choose an item" << std::endl;
				std::cin >> useItem;
				if (useItem == "key" && choice == 6) {
					choice == 13
				}
				else if (choice == 0) {

				}
				else if (choice == 1) {

				}
				else if (choice == 11) {

				}
				else if (choice == 12) {
					int reaction;
					if (useItem == "somethingWeird") {
						reaction = rand() % 4 + 1;
					}
					else {
						for (int i = 0; i < 11; i++) {
							if (useItem == itemname[i]) {
								reaction = react[i];
							}
						}
					}
					if (reaction == 1) {
						std::cout << "The guy did not like it so much that he hit you in the face(" << std::endl;
						hp = hp-20;
					}
					else if (reaction == 2) {
						std::cout << "The guy did not like it" << std::endl;
					}
					else if (reaction == 3) {
						std::cout << "The guy liked it" << std::endl;
					}
					else if (reaction == 4) {
						std::cout << "The guy liked it so much that he hugged and kissed you)" << std::endl;
						hp = hp+20;
					}
				}
				else {
					std::cout << "nothing to use here" << std::endl;
				}
			}
		}
		else if (command == "combine") {
			std::string item1;
			std::string item2;
			if (myItems.size() < 2) {
				std::cout << "nothing to combine" << std::endl;
			}
			else {
				std::cout << "choose the first item" << std::endl;
				std::cin >> item1;
				std::cout << "choose the second item" << std::endl;
				std::cin >> item2;
			}
		}
		else {
			std::cout << "there is no such command" << std::endl;
		}
	}
	if (feed == 50) {
		std::cout << "That guy has exploded. You are free to go." << std::endl;
	}
}
