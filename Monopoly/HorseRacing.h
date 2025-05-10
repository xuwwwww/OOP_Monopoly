#pragma once
#include <iostream>
#include "Player.h"

class Player;

using namespace std;

class HorseRacing {
public:
    HorseRacing();
    void init(Player* player);
    void gameStart();
private:
    class Horse {
    public:
        Horse();
        Horse(const char& level, const string& name);
        char run();
        string getName() const;
        char getLevel();
    private:
        char speed;
        string name;
        char mood;
        char condition;
        char level;
    };

    Player* player;
    Horse* horses;
    int length;
    char choosedHorse;
    int betMoney;
    double* odds;
};