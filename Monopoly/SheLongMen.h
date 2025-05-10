#pragma once
#include <vector>
#include "Player.h"

class Card {
public:
    Card() : face(-1), suit(-1) {}
    Card(const int& _face, const int& _suit) : face(_face), suit(_suit) {}
    int getFace() const { return face; }
    int getSuit() const { return suit; }
private:
    int face;
    int suit;
};

class Cards {
public:
    Cards() {
        init();
    }
    void init() {
        cards.clear();
        for (int face = 0; face < 13; ++face) {
            for (int suit = 0; suit < 4; ++suit) {
                cards.emplace_back(Card(face, suit));
            }
        }
    }
    Card randomDrawCard() {
        int index = rand() % cards.size();
        Card card = cards[index];
        cards.erase(cards.begin() + index);
        return card;
    }
private:
    std::vector<Card> cards;
};

class SheLongMen {
public:
    SheLongMen();
    void init(Player* player);
    void gameStart();
private:
    int card1, card2, nextCard;
    Cards poker;
    Player* player;
    int betMoney;
};

