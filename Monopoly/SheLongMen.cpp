#include "SheLongMen.h"
#include "Monopoly.h"

std::string face2String(const int& face) {
    switch (face) {
    case 0:
        return "A";
    case 1:
        return "2";
    case 2:
        return "3";
    case 3:
        return "4";
    case 4:
        return "5";
    case 5:
        return "6";
    case 6:
        return "7";
    case 7:
        return "8";
    case 8:
        return "9";
    case 9:
        return "10";
    case 10:
        return "J";
    case 11:
        return "Q";
    case 12:
        return "K";
    }
    return "?";
}

SheLongMen::SheLongMen() : poker(), player(nullptr), betMoney(0), card1(0), card2(0), nextCard(0) {}

void SheLongMen::init(Player* player) {
    poker.init();
    card1 = poker.randomDrawCard().getFace();
    card2 = poker.randomDrawCard().getFace();
    nextCard = poker.randomDrawCard().getFace();
    this->player = player;

    std::vector<std::string> bets = {
        "免費",
        "100元",
        "200元",
        "ALLIN"
    };
    std::string welcome("歡迎遊玩射龍門\n選擇下注的金額");
    bool flag = true;
    do {
        switch (Monopoly::GetUserChoice(welcome, bets)) {
        case 0:
            betMoney = 0;
            break;
        case 1:
            betMoney = 100;
            break;
        case 2:
            betMoney = 200;
            break;
        case 3:
            betMoney = player->GetMoney();
            break;
        }

        flag = player->GetMoney() < betMoney;
        if (flag)
            welcome += "\n金錢不夠下注";
    } while (flag);
}

void SheLongMen::gameStart() {
    std::string notify = "射龍門\n初始牌: " + face2String(card1) + ", " + face2String(card2);

    if (card1 != card2) {
        int low = std::min(card1, card2);
        int high = std::max(card1, card2);

        int choice = Monopoly::GetUserChoice(notify, {
            "範圍內",
            "範圍外"
            });

        std::cout << "第三張牌: " << face2String(nextCard) << std::endl;

        if (nextCard == card1 || nextCard == card2) {
            std::cout << "撞柱！損失兩倍賭注。" << std::endl;
            player->Pay(&Monopoly::bank, betMoney << 1);
        }
        else if ((nextCard > low && nextCard < high && choice == 0) ||
            ((nextCard < low || nextCard > high) && choice == 1)) {
            std::cout << "你贏了！ 獲得" << betMoney << "元！" << std::endl;
            Monopoly::bank.Pay(player, betMoney);
        }
        else {
            std::cout << "你輸了！ 損失" << betMoney << "元！" << std::endl;
            player->Pay(&Monopoly::bank, betMoney);
        }

    }
    else { // 撲克牌相同
        int choice = Monopoly::GetUserChoice(notify, {
            "小",
            "大"
            });

        std::cout << "第三張牌: " << face2String(nextCard) << std::endl;

        if (nextCard == card1) {
            std::cout << "撞柱！損失三倍賭注。" << std::endl;
            player->Pay(&Monopoly::bank, (betMoney << 1) + betMoney);
        }
        else if ((nextCard > card1 && choice == 1) ||
            (nextCard < card1 && choice == 0)) {
            std::cout << "你贏了！ 獲得" << betMoney << "元！" << std::endl;
            Monopoly::bank.Pay(player, betMoney);
        }
        else {
            std::cout << "你輸了！ 損失" << betMoney << "元！" << std::endl;
            player->Pay(&Monopoly::bank, betMoney);
        }
    }
}