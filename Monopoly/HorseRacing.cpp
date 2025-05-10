#include "HorseRacing.h"
#include "Monopoly.h"

HorseRacing::HorseRacing() : horses(new Horse[4]), betMoney(0), player(nullptr), length(0), choosedHorse(0) {
    odds = new double[4];
}

void HorseRacing::init(Player* player) {
    this->player = player;

    horses[0] = Horse(1, "A");
    horses[1] = Horse(2, "B");
    horses[2] = Horse(3, "C");
    horses[3] = Horse(4, "D");

    vector<string> bets = {
        "免費",
        "100元",
        "200元",
        "ALLIN"
    };
    string welcome;

    switch (rand() % 3) {
    case 0:
        welcome = "賭馬\n賭馬卷購買，跑道短";
        length = 20;
        break;
    case 1:
        welcome = "賭馬\n賭馬卷購買，跑道中";
        length = 50;
        break;
    case 2:
        welcome = "賭馬\n賭馬卷購買，跑道長";
        length = 100;
        break;
    }

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

        flag = !player->BuyProperty(betMoney);
        if (flag)
            welcome += "\n金錢不夠，請選便宜的賽馬卷";
    } while (flag);

    for (int i = 0; i < 4; ++i) {
        int gt(0), eq(0), lt(0);
        for (int j = 0; j < 4; ++j) {
            if (i == j) continue;
            if (horses[i].getLevel() != horses[j].getLevel()) {
                if (horses[i].getLevel() > horses[j].getLevel()) ++gt;
                else ++lt;
            }
            else {
                ++eq;
            }
        }
        if (eq == 3)
            odds[i] = 1.0;
        else if (gt == 3)
            odds[i] = 0.5;
        else if (lt == 3)
            odds[i] = 5.0;
        else
            odds[i] = lt;
    }

    vector<string> choices;
    for (int i = 0; i < 4; ++i) {
        choices.emplace_back("馬" + horses[i].getName() + ", 賠率: " + to_string(odds[i]) + "倍");
    }

    choosedHorse = Monopoly::GetUserChoice("選擇馬", choices);
}

void HorseRacing::gameStart() {
    vector<int> positions(4, 0); // 各馬的目前距離
    bool finished = false;
    int winner = -1;

    while (!finished) {
        // 每匹馬移動
        for (int i = 0; i < 4; ++i) {
            positions[i] += horses[i].run();
            if (positions[i] >= length && !finished) {
                finished = true;
                winner = i;
            }
        }

        system("cls");

        cout << "賽馬進行中...\n\n";

        for (int i = 0; i < 4; ++i) {
            cout << horses[i].getName() << "馬 |";

            int pos = min(positions[i], length);
            for (int j = 0; j < pos; ++j) cout << '-';
            cout << '>';
            for (int j = pos; j < length; ++j) cout << ' ';
            cout << "|\n";
        }

        Monopoly::sleepMS(300);
    }

    cout << "\n賽事結束！獲勝的是 馬" << horses[winner].getName() << "！\n";

    if (choosedHorse == winner) {
        cout << "你贏了！賺到 $" << betMoney * odds[winner] << '\n';
        Monopoly::bank.Pay(player, betMoney * (1 + odds[winner]));
    }
    else {
        cout << "你輸了！損失 $" << betMoney << '\n';
    }
}

HorseRacing::Horse::Horse() : level(0), speed(0), mood(0), condition(0), name("\0") {}

HorseRacing::Horse::Horse(const char& level, const string& name) : level(level), speed((rand() % 3 + 1)* level + level), mood(rand() % 10 + 1), condition(rand() % 7 + 5), name(name) {}

char HorseRacing::Horse::run() {
    if (mood == 0)
        return 0;
    char result;
    int r = rand() % 100;
    int fullRunChance = mood * 10; // 全速跑機率
    int halfRunChance = (100 - fullRunChance) / 2; // 跑一半機率
    int noRunChance = 100 - fullRunChance - halfRunChance; // 不跑機率

    if (r < fullRunChance) {
        result = speed;
    }
    else if (r < fullRunChance + halfRunChance) {
        result = speed >> 1;
    }
    else {
        return 0;
    }
    return result * condition / 10;
}

string HorseRacing::Horse::getName() const {
    return name;
}

char HorseRacing::Horse::getLevel() {
    return level;
}