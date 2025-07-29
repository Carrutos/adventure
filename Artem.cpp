
#include <vector>
#include <iostream>
using namespace std;
struct door {
	string name;
	int target;
};

struct room {
	string name;
	string desc;
	vector<door>doors;
	vector<string>items;
};

struct useResult {
	std::string start;
	std::string end;
};

/*struct combineResult {
	std::string item1;
	std::string item2;
	std::string result;
};*/

struct human {
	string name;
	int hp;
	int damage;
};

int main()
{
	std::string itemname[36] = { "chocolate", "frozenPelmeni", "carrot", "apple", "tomato", "water", "milk", "jam", "dryNoodles", "frozenFish", "lemon", "key", "moltenChocolate",
		"friedPelmeni", "friedCarrot", "friedApple", "tomatoJuice", "butter", "friedNoodles", "friedFish", "copper", "coal", "frozenChocolate", "ice", "frozenCarrot", "frozenApple",
		"frozenTomato", "milkIce", "jamIce", "frozenNoodles", "frozenLemon", "frozenKey", "chocoMass", "frozenJuice", "frozenButter", "somethingWeird" };
	int react[35] = { 4, 1, 3, 3, 2, 2, 3, 3, 2, 2, 2, 1, 2, 4, 3, 2, 2, 2, 3, 3, 1, 1, 2, 2, 2, 2, 2, 3, 2, 1, 2, 1, 2, 2, 2 };
	room list[13];
	list[0].name = { "oven" };
	list[0].desc = { "cooks food" };
	list[1] = { "brokenStove","does not cook food" };
	list[1].items = { itemname[3] };
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
	list[0].doors = { { "oven-brokenStove",1 },{ "oven-fridge",2 },{ "oven-kitchenHall",4 } };
	list[1].doors = { { "brokenStove-oven",0 },{ "brokenStove-closet",3 },{ "brokenStove-kitchenHall",4 } };
	list[2].doors = { { "fridge-oven",0 },{ "fridge-kitchenHall",4 } };
	list[2].items = { itemname[4], itemname[3], itemname[6] };
	list[3].doors = { { "closet-brokenStove",1 },{ "closet-kitchenHall",4 } };
	list[3].items = { itemname[2], itemname[8] };
	list[4].doors = { { "kitchenHall-oven",0 },{ "kitchenHall-brokenStove",1 },{"kitchenHall-fridge",2},{"kitchenHall-closet",3},{"kitchenHall-hall",5},
		{"kitchenHall-storageRoom",7},{"kitchenHall-man",12} };
	list[4].items = { itemname[0] };
	list[5].doors = { { "hall-kitchenHall",4 },{ "hall-exit",6 } };
	list[5].items = { itemname[11] };
	list[6].doors = { { "exit-hall",5 } };
	list[7].doors = { { "storageRoom-kitchenHall",4 },{ "storageRoom-pool",8 },{"storageRoom-closet1",9},{"storageRoom-closet2",10},{"storageRoom-iceRoom",11} };
	list[7].items = { itemname[0] };
	list[8].doors = { { "pool-storageRoom",7 } };
	list[8].items = { itemname[5] };
	list[9].doors = { { "closet1-storageRoom",7 },{ "closet1-closet2",10} };
	list[9].items = { itemname[7], itemname[10] };
	list[10].doors = { { "closet2-storageRoom",7 },{ "closet2-closet1",9} };
	list[10].items = { itemname[2], itemname[8] };
	list[11].doors = { { "iceRoom-storageRoom",7 } };
	list[11].items = { itemname[1], itemname[9] };
	list[12].doors = { { "man-kitchenHall",4 } };
	useResult useOven[30] = { {"chocolate", "moltenChocolate"}, {"frozenPelmeni", "friedPelmeni"}, {"carrot", "friedCarrot"}, {"apple", "friedApple"}, {"tomato", "tomatoJuice"},
		{"milk", "butter"}, {"jam", "jam"}, {"dryNoodles", "friedNoodles"}, {"frozenFish", "friedFish"}, {"lemon", "somethingWeird"}, {"key", "copper"}, {"moltenChocolate", "coal"},
		{"tomatoJuice", "tomatoJuice"}, {"butter", "butter"}, {"copper", "copper"}, {"coal", "coal"}, {"frozenChocolate", "chocolate"}, {"ice", "water"}, {"frozenCarrot", "carrot"},
		{"frozenApple", "apple"}, {"frozenTomato", "tomato"}, {"milkIce", "milk"}, {"jamIce", "jam"}, {"frozenNoodles", "friedNoodles"}, {"frozenLemon", "lemon"}, {"frozenKey", "key"},
		{"chocoMass", "moltenChocolate"}, {"frozenJuice", "tomatoJuice"}, {"frozenButter", "butter"}, {"somethingWeird", "somethingWeird"}};
	useResult useIce[25] = { {"chocolate", "frozenChocolate"}, {"carrot", "frozenCarrot"}, {"apple", "frozenApple"}, {"tomato", "frozenTomato"}, {"water", "ice"}, {"milk", "milkIce"},
		{"jam", "jamIce"}, {"dryNoodles", "frozenNoodles"}, {"lemon", "frozenLemon"}, {"key", "frozenKey"}, {"moltenChocolate", "chocoMass"}, {"friedPelmeni", "frozenPelmeni"},
		{"friedCarrot", "frozenCarrot"}, {"friedApple", "frozenApple"}, {"tomatoJuice", "frozenJuice"}, {"butter", "frozenButter"}, {"friedNoodles", "frozenNoodles"}, {"friedFish",
		"frozenFish"}, {"copper", "copper"}, {"coal", "coal"}, {"ice", "ice"}, {"milkIce", "ice"}, {"jamIce", "ice"}, {"chocoMass", "frozenChocolate"}, {"somethingWeird",
		"somethingWeird"} };
	//combineResult combines = {};
	int choice = 4;
	int hp = 81;
	int damage = 20;
	std::vector<std::string>myItems;
	std::string command;
	int feed = 0;
	human man = { "man", 21, 10 };
	human aqua = { "aquaman", 21, 5 };
	human skelet = { "skeleton", 11, 5 };
	std::cout << "you wake up in the kitchen of some fat guy's house and are told to feed him something" << std::endl;
	std::cout << "Be careful! Nasty monsters are somewhere in the house!" << std::endl;
	while (hp > 0 || feed < 50) {
		std::cout << "you are in " << list[choice].name << std::endl;
		std::cin >> command;
		if (command == "go") {
			std::cout << "where do you wanna go (pick a number)" << std::endl;
			for (int i = 0; i < list[choice].doors.size(); i++) {
				std::cout << list[choice].doors[i].target << " - " << list[list[choice].doors[i].target].name << std::endl;
			}
			int choiceOld = choice;
			std::cin >> choice;
			int counter = 0;
			for (int i = 0; i < list[choiceOld].doors.size(); i++) {
				if (choice == list[choiceOld].doors[i].target) {
					counter++;
				}
			}
			if (counter == 0) {
				choice == choiceOld;
				std::cout << "you cannot go there" << std::endl;
			}
			if (choice == 11) {
				hp = hp - 2;
			}
			if (choice == 8 && aqua.hp > 0 || choice == 10 && skelet.hp > 0) {
				int ahp;
				int adamage;
				if (choice == 8) {
					std::cout << "aquaman hits you" << std::endl;
					ahp = aqua.hp;
					adamage = aqua.damage;
					hp = hp - adamage;
				}
				if (choice == 10) {
					std::cout << "skeleton in the closet hits you" << std::endl;
					ahp = skelet.hp;
					adamage = skelet.damage;
					hp = hp - adamage;
				}
				string decision;
				while (decision != "out") {
					cout << "hit - hit him as revenge, pass - go away" << std::endl;
					cin >> decision;
					if (decision == "hit") {
						std::cout << "you hit him" << std::endl;
						std::cout << "he hit you" << std::endl;
						ahp = ahp -damage;
						hp = hp - adamage;
						std::cout << hp << std::endl;
						if (ahp >= 1) {
							std::cout << ahp << std::endl;
						}
						else {
							std::cout << 0 << std::endl;
						}
					}
					if (decision == "pass") {
						break;
					}
					if (ahp < 1) {
						std::cout << "he is no more" << std::endl;
						decision = "out";
					}
					else if (hp < 1) {
						std::cout << "rest in peace" << std::endl;
						decision = "out";
					}
				}
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
					myItems.push_back(itemname[5]);
					std::cout << "item picked successfully" << std::endl;
				}
				else {
					for (int i = 0; i < list[choice].items.size(); i++) {
						if (pickItem == list[choice].items[i]) {
							myItems.push_back(list[choice].items[i]);
							list[choice].items.erase(list[choice].items.cbegin() + i);
							std::cout << "item picked successfully" << std::endl;
						}
					}
				}
				if (pickItem == "somethingWeird") {
					damage = damage + 2;
					std::cout << "damage is up " << damage << std::endl;
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
				int counter = 0;
				for (int i = 0; i < myItems.size(); i++) {
					if (dropItem == myItems[i]) {
						list[choice].items.push_back(myItems[i]);
						myItems.erase(myItems.cbegin() + i);
						std::cout << "item dropped successfully" << std::endl;
						counter++;
					}
				}
				if (counter == 0) {
					std::cout << "get it first >>" << std::endl;
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
					break;
				}
				else if (choice == 0) {
					if (useItem == itemname[13] || useItem == itemname[14] || useItem == itemname[15] || useItem == itemname[18] || useItem == itemname[19]) {
						for (int j = 0; j < myItems.size(); j++) {
							if (useItem == myItems[j]) {
								myItems[j] = "coal";
								std::cout << "you now have coal" << std::endl;
							}
						}
					}
					else if (useItem == "water") {
						for (int j = 0; j < myItems.size(); j++) {
							if (useItem == myItems[j]) {
								myItems.erase(myItems.cbegin() + j);
								std::cout << "you now have nothing" << std::endl;
							}
						}
					}
					else {
						for (int i = 0; i < 30; i++) {
							if (useOven[i].start == useItem) {
								for (int j = 0; j < myItems.size(); j++) {
									if (useItem == myItems[j]) {
										myItems[j] = useOven[i].end;
										std::cout << "you now have " << useOven[i].end << std::endl;
									}
								}
							}
						}
					}
				}
				else if (choice == 4) {
					if (useItem != itemname[11] && useItem != itemname[20] && useItem != itemname[21] && useItem != itemname[33]) {
						for (int j = 0; j < myItems.size(); j++) {
							if (useItem == myItems[j]) {
								myItems.erase(myItems.cbegin() + j);
								hp = hp + 10;
								std::cout << "hp is up" << std::endl;
							}
						}
					}
					else {
						std::cout << "you cannot eat this" << std::endl;
					}
				}
				else if (choice == 11) {
					if (useItem == itemname[1] || useItem == itemname[9] || useItem == itemname[22] || useItem == itemname[24] || useItem == itemname[25] || useItem == itemname[26]
						|| useItem == itemname[29] || useItem == itemname[30] || useItem == itemname[31] || useItem == itemname[33] || useItem == itemname[34]) {
						for (int j = 0; j < myItems.size(); j++) {
							if (useItem == myItems[j]) {
								std::cout << "you still have " << useItem << std::endl;
							}
						}
					}
					else {
						for (int i = 0; i < 25; i++) {
							if (useIce[i].start == useItem) {
								for (int j = 0; j < myItems.size(); j++) {
									if (useItem == myItems[j]) {
										myItems[j] = useIce[i].end;
										std::cout << "you now have " << useIce[i].end << std::endl;
									}
								}
							}
						}
					}
				}
				else if (choice == 12) {
					int reaction = 0;
					if (useItem == "somethingWeird") {
						reaction = 1 + rand() % 4;
					}
					else {
						for (int i = 0; i < 23; i++) {
							if (useItem == itemname[i]) {
								reaction = react[i];
							}
						}
					}
					if (reaction == 1) {
						std::cout << "The guy did not like it so much that he hit you in the face" << std::endl;
						hp = hp - man.damage;
						std::string decision;
						while (decision != "out") {
							std::cout << "hit - hit him as revenge, pass - go away" << std::endl;
							std::cin >> decision;
							if (decision == "hit") {
								std::cout << "you hit him" << std::endl;
								std::cout << "he hit you in the face" << std::endl;
								man.hp = man.hp - 20;
								man.damage = man.damage - 5;
								hp = hp - man.damage;
								std::cout << hp << std::endl;
								if (man.hp >= 1) {
									std::cout << man.hp << std::endl;
								}
								else {
									std::cout << 0 << std::endl;
								}
							}
							if (decision == "pass") {
								break;
							}
							if (man.hp < 1) {
								std::cout << "he is no more" << std::endl;
								decision = "out";
							}
							else if (hp < 1) {
								std::cout << "rest in peace" << std::endl;
								decision = "out";
							}
						}
						if (man.hp < 1) {
							break;
						}
					}
					else if (reaction == 2) {
						std::cout << "The guy did not like it" << std::endl;
					}
					else if (reaction == 3) {
						std::cout << "The guy liked it" << std::endl;
					}
					else if (reaction == 4) {
						std::cout << "The guy liked it so much that he hugged and kissed you)" << std::endl;
						hp = hp + 20;
					}
					for (int j = 0; j < myItems.size(); j++) {
						if (useItem == myItems[j]) {
							myItems.erase(myItems.cbegin() + j);
						}
					}
					feed++;
				}
				else {
					std::cout << "nothing to use here" << std::endl;
				}
			}
		}
		/*else if (command == "combine") {
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
			for (int i = 0; i < 20; i++) {
				if (combines[i].item1 == item1 && combines[i].item2 == item2) {
					myItems[j].push_back(combines[i].result;
					std::cout << "you now have" << combines[i].result << std::endl;
				}
			}
		}*/
		else {
			std::cout << "there is no such command" << std::endl;
		}
	}
	if (feed == 50) {
		std::cout << "That guy has exploded. You are free to go." << std::endl;
	}
}
