#include "Monopoly.h"
#include "Game.h"
#include "CommandHandler.h"
#include <chrono>
#include <conio.h>
#include <ctime>
#include <thread>
#include <iostream>
#include <memory>

namespace Monopoly {
	Game* game = nullptr;
	Player bank("bank", "black", 1 << 30);
}

std::string Monopoly::GetColorCode(std::string colorName) {
	if (colorName == "red")		return "\033[31m";
	if (colorName == "green")	return "\033[32m";
	if (colorName == "yellow")	return "\033[33m";
	if (colorName == "blue")	return "\033[34m";
	return "\033[0m"; // 預設
}

std::string Monopoly::GetBackgroundColorCode(std::string color) {
	if (color == "red")        return "\033[41;37m";
	if (color == "green")      return "\033[42;37m";
	if (color == "yellow")     return "\033[43;30m";
	if (color == "blue")       return "\033[44;37m";
	if (color == "cyan")       return "\033[46;30m";
	if (color == "purple")     return "\033[48;2;128;0;128m\033[37m";
	if (color == "orange")     return "\033[48;2;255;140;0m\033[37m";
	if (color == "teal")       return "\033[48;2;0;128;128m\033[37m";
	if (color == "brown")      return "\033[48;2;139;69;19m\033[37m";
	if (color == "gray")   return "\033[100;37m";
	return "\033[0m"; // 預設
}

void Monopoly::SetTextColor(const Color& color) {
	printf("\033[%dm", color);
}

void Monopoly::SetBackgroundColor(const BackgroundColor& color) {
	printf("\033[;%dm", color);
}

void Monopoly::UpdateScreen() {
	system("cls");
	game->PrintMapStatus();
	game->PrintPlayerStatus();
}

// withMap參數，true 顯示地圖和玩家， false 不顯示，想要有地圖用Monopoly::GetUserChoice(question, options, true);
int Monopoly::GetUserChoice(const std::string& question, const std::vector<std::string>& options, bool withMap, bool cmd) {
	int selected = 0;
	int key;

	while (_kbhit()) _getch();

	do {
		if (withMap) game->Clear();
		else { // 根據withMap參數，true 顯示地圖和玩家， false 不顯示
			system("cls");
		}

		std::cout << question << "\n\n";
		for (size_t i = 0; i < options.size(); ++i) {
			if (static_cast<int>(i) == selected)
				std::cout << " > " << "【" << options[i] << "】" << "\n";
			else
				std::cout << "   " << "【" << options[i] << "】" << "\n";
		}

		if (cmd) {
			std::cout << "\n請使用上下鍵選擇，按 Enter 確認，或按 / 輸入指令" << std::endl;
		}
		else {
			std::cout << "\n請使用上下鍵選擇，按 Enter 確認" << std::endl;
		}

		key = _getch();
		if (key == 224) {
			key = _getch();
			if (key == 72) selected = (selected - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
			if (key == 80) selected = (selected + 1) % static_cast<int>(options.size());
		}
		else if (cmd && key == '/') {
			// Command input mode
			std::string input = Monopoly::tappingCommand();

			std::cout << std::endl;

			// Convert current player to shared_ptr for command processing
			Player* currentPlayer = game->getCurrentPlayer();
			if (currentPlayer) {
				std::shared_ptr<Player> playerPtr(currentPlayer, [](Player*) {}); // No-op deleter - we don't own the player

				// Process the command
				std::pair<bool, int> result = game->processCommand(playerPtr, input);
				if (result.first) {
					// Command w processed, wait for user to acknowledge
					std::cout << "\n指令已執行。按任意鍵繼續..." << std::endl;
					_getch();
					if (result.second == 1)return -1;
				}
				else {
					// Command failed
					std::cout << "\n無效的指令。按任意鍵繼續..." << std::endl;
					_getch();
				}
			}

			// Continue with the menu
			continue;
		}

	} while (key != '\r');

	//	else if (key == '\r') {
	//	if (cmd) {
	//		// 判斷是否是擲骰子選項，如果是則直接返回
	//		if (options.size() > 0 && selected == 0 && options[0] == "進行擲骰") {
	//			return selected;
	//		}

	//		// 改為詢問是否輸入指令
	//		std::string input;
	//		std::cout << "\n請按 Enter 選擇，或輸入指令（以 / 開頭）: ";
	//		std::getline(std::cin, input);

	//		if (!input.empty() && input[0] == '/') {
	//			// Convert current player to shared_ptr for command processing
	//			Player* currentPlayer = game->getCurrentPlayer();
	//			if (currentPlayer) {
	//				std::shared_ptr<Player> playerPtr(currentPlayer, [](Player*) {}); // No-op deleter

	//				// Try to process the command with our new system first
	//				if (game->processCommand(playerPtr, input)) {
	//					// Wait for user acknowledgment
	//					std::cout << "\n指令已執行。按任意鍵繼續..." << std::endl;
	//					_getch();
	//					continue;
	//				}
	//			}

	//			throw;
	//		}
	//		else {
	//			// 空字串代表按下 Enter，不輸入指令
	//			return selected;
	//		}
	//	}
	//	else return selected;
	//}


	return selected;
}

void Monopoly::WaitForEnter() {
	std::cout << "\n按下 Enter 鍵繼續...\n";
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

std::string Monopoly::tappingCommand() {
	std::string input = "/";
	std::cout << "\n指令：" << input;

	// Keep reading input until Enter is pressed
	int ch;
	while (ch = _getch(), ch != '\r') {
		if (ch == '\b') { // Backspace
			if (input.length() > 1) { // Don't delete the '/'
				input.pop_back();
				std::cout << "\b \b"; // Erase character on screen
			}
		}
		else if (ch >= 32 && ch <= 126) { // Printable characters
			input += (char)ch;
			std::cout << (char)ch;
		}
	}

	return input;
}
