
#include <vector>
#include <iostream>




struct food {
    std::string name;
    int amount;
    int price;
};


struct boughtitem {
    std::string name;
    int amount;
};



void rsp()
{
    std::string usertool;
    std::string alltool[3]{ "rock","scissors","paper" };
    for (int i = 0;i < 10;i++) {
        std::string onetool = alltool[rand() % 3];
        std::cout << onetool << std::endl;

        std::cin >> usertool;
        if (usertool == "rock" && onetool == "scissors" ||
            usertool == "scissors" && onetool == "paper" ||
            usertool == "paper" && onetool == "rock") {
            std::cout << "You win!\n";
        }
        else if (usertool == onetool) {
            std::cout << "Draw!\n";
        }
        else if (usertool != "rock" && usertool != "scissors" && usertool != "paper") {
            std::cout << "Try again!\n";
        }
        else {
            std::cout << "You lose!\n";
        }
    }
}


int main()
{
   /* int randnum = rand() % 5;
    int usernum;
    for (int i=0;i < 10;i++) {
        std::cin >> usernum;
        if (randnum == usernum) {
            std::cout << "You win!\n";
            randnum = rand() % 9;
        }
        else {
            std::cout << "You lose!\n";
        }
    }*/


    std::vector<food>list;
    list.push_back({ "stick", 7, 70 });
    list.push_back({"tomato", 5, 100});
    list.push_back({ "banana", 3, 150 });
    list.push_back({ "avocado", 2, 250 });
    list.push_back({ "porno", 2, 5000 });
    std::string act;
    int money = 4950;



    
    std::vector<boughtitem>boughtlist;



    while (act != "out") {
        for (int i = 0; i < list.size(); i++) {
            std::cout << list[i].name << " " << list[i].amount << " " << list[i].price << std::endl;
        }
        std::cout << money << std::endl;
        std::cin >> act;
        if (act == "buy") {
            std::cout << "buy what" << std::endl;
            std::string userprod;
            std::cin >> userprod;
            int chosen = 5;
            for (int i = 0;i < list.size();i++) {
                if (list[i].name == userprod) {
                    chosen = i;
                    if (list[chosen].amount != 0) {
                        std::cout << "how much?" << std::endl;
                        int useram;
                        std::cin >> useram;
                        if (money >= useram * list[chosen].price && useram <= list[i].amount) {
                            std::cout << "success" << std::endl;
                            list[chosen].amount -= useram;
                            money -= useram * list[chosen].price;
                            boughtlist.push_back({ userprod, useram });
                        }
                        else {
                            std::cout << "too much" << std::endl;
                        }
                    }
                    else {
                        std::cout << "not in stock" << std::endl;
                    }
                }
            }
            if (chosen == 5) {
                std::cout << "what is that?" << std::endl;
            }
        }
        else if (act == "maxmoney") {
            int alot = money;
            for (int i = list.size()-1; alot >= 0 && i >= 0; i--) {
                for (int j = 0; alot >= 0 && j < list[i].amount; j++) {
                    if (alot - list[i].price > 0) {
                        alot -= list[i].price;
                        std::cout << list[i].name << std::endl;
                    }
                }
            }
        }
        else if (act == "maxnum") {
            int alot = money;
            for (int i = 0; alot >= 0 && i < list.size(); i++) {
                for (int j = 0; alot >= 0 &&  j < list[i].amount; j++) {
                    if (alot - list[i].price > 0) {
                        alot -= list[i].price;
                        std::cout << list[i].name << std::endl;
                    }
                }
            }
        }
        else if (act == "bought") {
            for (int i = 0; i < boughtlist.size();i++) {
                std::cout << boughtlist[i].name << std::endl;
                std::cout << boughtlist[i].amount << std::endl;
            }
        }
        else if (act == "out") {
            std::cout << "bye" << std::endl;
        }
        else {
            std::cout << "no such command" << std::endl;
        }
    }
}


