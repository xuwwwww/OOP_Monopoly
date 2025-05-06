#include "Game.h"
#include "Map.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>

std::string colors[4] = { "red", "yellow", "green", "blue" };

std::vector<std::vector<std::string>> diceFaces = {
	{
		"┌───────┐",
		"│       │",
		"│   ●   │",
		"│       │",
		"└───────┘"
	},
	{
		"┌───────┐",
		"│ ●     │",
		"│       │",
		"│     ● │",
		"└───────┘"
	},
	{
		"┌───────┐",
		"│ ●     │",
		"│   ●   │",
		"│     ● │",
		"└───────┘"
	},
	{
		"┌───────┐",
		"│ ●   ● │",
		"│       │",
		"│ ●   ● │",
		"└───────┘"
	},
	{
		"┌───────┐",
		"│ ●   ● │",
		"│   ●   │",
		"│ ●   ● │",
		"└───────┘"
	},
	{
		"┌───────┐",
		"│ ●   ● │",
		"│ ●   ● │",
		"│ ●   ● │",
		"└───────┘"
	}
};

Game::Game()
{
	currentPlayerIdx = 0;
	gameOver = false;
	srand(static_cast<unsigned int>(time(nullptr)));
}

void Game::InitGame()
{
	gameMap = new Map();

	int playerNum = 0;

	std::string question = "## 選擇遊玩人數 ##";
	std::vector<std::string> options = {
		"2人",
		"3人",
		"4人"
	};

	int choice = GetUserChoice(question, options);

	playerNum = choice + 2;

	Clear();
	std::cout << "## 輸入玩家設定 ##\n";
	for (int i = 0; i < playerNum; i++) {
		std::string name;
		std::cout << "\n玩家 " << i + 1 << " 的名稱：";
		std::cin >> name;
		Player* p = new Player(name, colors[i], 1000);
		players.push_back(p);
	}


	std::cout << "\n遊戲初始化完成，共 " << players.size() << " 位玩家。" << std::endl;

	Clear(); // 在玩家設定完成後清除畫面並顯示初始狀態
	WaitForEnter();
}

void Game::StartGame()
{
	while (!gameOver) {
		NextTurn();
		CheckWinCondition();
	}
	EndGame();
}

void Game::NextTurn()
{
	auto currentPlayer = players[currentPlayerIdx];
	
	// 如果當前玩家已破產，跳到下一位
	if (currentPlayer->GetMoney() <= 0) {
		// 換下一位
		currentPlayerIdx = (currentPlayerIdx + 1) % static_cast<int>(players.size());
		return;
	}
	
	std::cout << "\n輪到 " << currentPlayer->GetName() << " 的回合。" << std::endl;
	if (currentPlayer->inHospital) {
		if (currentPlayer->hosipitalDay >= 3) {
			std::cout << "\n出院了！";
		}
		else {
			std::cout << "\n輸入 r 進行擲骰，若點數為 6 則提出院！";
		}
	}

	std::string question = "輪到 " + currentPlayer->GetName() + " 的回合。\n";
	std::vector<std::string> options = {
		"進行擲骰",
		"使用道具"
	};
	int choice = GetUserChoice(question, options);

	if (choice == 0) {
		int roll = RollDiceWithAsciiAnimation();
		currentPlayer->Move(roll, gameMap->getSize());
		int pos = currentPlayer->GetPosition();
		std::cout << currentPlayer->GetName() << " 移動到第 " << pos << " 格。\n";

		auto currentTile = gameMap->GetTileAt(pos);

		currentTile->OnLand(currentPlayer);
	}
	else if (choice == 1) {
		// 使用道具的邏輯
	}

	WaitForEnter();

	// 換下一位
	currentPlayerIdx = (currentPlayerIdx + 1) % static_cast<int>(players.size());
}

bool Game::CheckWinCondition()
{
	// 計算仍在遊戲中的玩家數
	int activePlayers = 0;
	int lastPlayerIdx = -1;
	
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i]->GetMoney() > 0) {
			activePlayers++;
			lastPlayerIdx = i;
		}
	}
	
	// 如果只剩一位玩家沒有破產，則他獲勝
	if (activePlayers == 1 && lastPlayerIdx != -1) {
		std::cout << "\n" << players[lastPlayerIdx]->GetName() << " 是唯一沒有破產的玩家，獲得勝利！" << std::endl;
		gameOver = true;
		return true;
	}
	
	// 檢查是否有玩家達到資金條件獲勝
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i]->GetMoney() >= 2000) {
			std::cout << "\n" << players[i]->GetName() << " 贏得勝利！" << std::endl;
			gameOver = true;
			return true;
		}
	}
	
	return false;
}

void Game::EndGame()
{
	std::cout << "\n遊戲結束，最終資金：" << std::endl;
	for (size_t i = 0; i < players.size(); i++) {
		std::cout << players[i]->GetName() << ": $" << players[i]->GetMoney() << std::endl;
	}
}

void Game::HandlePlayerAction(Player* p)
{
}

void Game::Clear()
{
	std::cout << "\033[2J\033[H"; // 清除畫面
	PrintMapStatus();
	PrintPlayerStatus();
	std::cout << std::endl;
}

void Game::PrintMapStatus()
{
	// 直接呼叫 Map 類別的 PrintMap 函式，使用新的網格佈局
	gameMap->PrintMap(players);
}

void Game::PrintPlayerStatus()
{
	for (size_t i = 0; i < players.size(); i++) {
		std::string colorCode = GetColorCode(players[i]->GetColor());
		std::cout << colorCode;
		std::cout << "玩家名稱：" << players[i]->GetName() << '\t' << "資金：" << players[i]->GetMoney() << '\n';
		std::cout << "\033[0m";
	}
}

void Game::PrintDice(int d1, int d2) {

	auto& face1 = diceFaces[d1 - 1];
	auto& face2 = diceFaces[d2 - 1];

	for (int i = 0; i < 5; i++) {
		std::cout << face1[i] << "  " << face2[i] << std::endl;
	}
}

int Game::RollDiceWithAsciiAnimation() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	const int rollTimes = 10;
	int finalRoll = 1;

	std::cout << "擲骰中...\n";

	for (int i = 0; i < rollTimes; ++i) {
		int d1 = rand() % 6 + 1;
		int d2 = rand() % 6 + 1;

		Clear();
		PrintDice(d1, d2);
		std::this_thread::sleep_for(std::chrono::milliseconds(75));
		finalRoll = d1 + d2;
	}

	std::cout << "擲骰結果是 " << finalRoll << " 點！" << std::endl;
	return finalRoll;
}


int Game::GetUserChoice(const std::string question, const std::vector<std::string> options) {

	int selected = 0;

	while (true) {
		Clear();
		std::cout << question << std::endl;
		for (size_t i = 0; i < options.size(); ++i) {
			if (static_cast<int>(i) == selected)
				std::cout << " > " << "【" << options[i] << "】" << "\n";
			else
				std::cout << "   " << "【" << options[i] << "】" << "\n";
		}

		int key = _getch(); // 取得按鍵
		if (key == 224) {   // 特殊按鍵
			key = _getch();
			if (key == 72) selected = (selected - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size()); // 上
			if (key == 80) selected = (selected + 1) % static_cast<int>(options.size()); // 下
		}
		else if (key == '\r') { // Enter
			return selected;
		}
	}
}

void Game::WaitForEnter()
{
	std::cout << "\n按下 Enter 鍵繼續...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string Game::GetColorCode(std::string colorName) {
	if (colorName == "red")		return "\033[31m";
	if (colorName == "green")	return "\033[32m";
	if (colorName == "yellow")	return "\033[33m";
	if (colorName == "blue")	return "\033[34m";
	return "\033[0m"; // 預設
}

std::string Game::GetBackgroundColorCode(std::string color) {
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

