#include "Monopoly.h"
#include "Game.h"
#include <chrono>
#include <conio.h>
#include <ctime>
#include <thread>
#include <iostream>

namespace Monopoly {
    Game* game = nullptr;
    Player bank("bank", "black", 1 << 30);
}

void Monopoly::setTextColor(const Color& color) {
	printf("\033[%dm", color);
}

void Monopoly::setBackgroundColor(const BackgroundColor& color) {
    printf("\033[;%dm", color);
}

int Monopoly::GetUserChoice(const std::string& question, const std::vector<std::string>& options, bool withMap) {
    int selected = 0;

    while (true) {
        if (withMap) game->Clear();
        else system("cls");
        
        std::cout << question << "\n\n";
        for (size_t i = 0; i < options.size(); ++i) {
            if (static_cast<int>(i) == selected)
                std::cout << " > " << "¡i" << options[i] << "¡j" << "\n";
            else
                std::cout << "   " << "¡i" << options[i] << "¡j" << "\n";
        }

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
            if (key == 80) selected = (selected + 1) % static_cast<int>(options.size());
        }
        else if (key == '\r') {
            return selected;
        }
    }
}

void Monopoly::WaitForEnter() {
    std::cout << "\n«ö¤U Enter ÁäÄ~Äò...";
    while (_getch() != '\r');
}

void Monopoly::clearScreen() {
    system("cls");
}

void Monopoly::sleepMS(const int& ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Monopoly::sleepS(const int& s) {
    std::this_thread::sleep_for(std::chrono::seconds(s));
}