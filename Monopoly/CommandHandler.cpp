#include "CommandHandler.h"
#include "Game.h"
#include "Monopoly.h"
#include "HorseRacing.h"
#include "SheLongMen.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../nlohmann/json.hpp"

// Using nlohmann json
using json = nlohmann::json;

CommandHandler::CommandHandler() : game(nullptr) {
}

// Helper function to find a player by name
Player* CommandHandler::FindPlayerByName(const std::string& name) {
	if (!game) return nullptr;

	// Search through all players
	for (auto& player : game->players) {
		if (player->GetName() == name) {
			return player;
		}
	}

	// Not found
	return nullptr;
}

void CommandHandler::Initialize() {
	// Try multiple possible paths for command.json
	std::vector<std::string> possiblePaths = {
		"../json/command.json",
		"json/command.json",
		"../../json/command.json",
		"../../../json/command.json",
		"./json/command.json"
	};

	bool fileOpened = false;
	std::ifstream file;

	for (const auto& path : possiblePaths) {
		file.open(path);
		if (file.is_open()) {
			std::cout << "成功開啟指令檔: " << path << std::endl;
			fileOpened = true;
			break;
		}
	}

	if (!fileOpened) {
		std::cerr << "錯誤: 找不到 command.json!" << std::endl;
		return;
	}

	try {
		json commandData;
		file >> commandData;

		// Parse commands from JSON
		commands.clear();
		for (const auto& cmd : commandData["commands"]) {
			Command command;
			command.name = cmd["name"];
			command.description = cmd["description"];
			command.usage = cmd["usage"];
			command.example = cmd["example"];
			commands.push_back(command);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "解析 command.json 時發生錯誤: " << e.what() << std::endl;
	}

	file.close();
}

bool CommandHandler::ProcessCommand(std::shared_ptr<Player> player, const std::string& input) {
	if (input.empty() || input[0] != '/') {
		return false;
	}

	// Split the command and its arguments
	std::vector<std::string> parts = SplitCommand(input);
	if (parts.empty()) {
		return false;
	}

	// Extract command name (without the /)
	std::string cmdName = parts[0].substr(1);
	if (cmdName.empty()) {
		return false;
	}

	// Remove command from arguments
	parts.erase(parts.begin());

	bool commandResult = false;

	// Find and execute the appropriate command handler
	if (cmdName == "move") {
		commandResult = HandleMoveCommand(player, parts);
	}
	else if (cmdName == "get") {
		commandResult = HandleGetCommand(player, parts);
	}
	else if (cmdName == "give") {
		commandResult = HandleGiveCommand(player, parts);
	}
	else if (cmdName == "card") {
		commandResult = HandleCardCommand(player, parts);
	}
	else if (cmdName == "gamestate") {
		commandResult = HandleGameStateCommand(player, parts);
	}
	else if (cmdName == "info") {
		commandResult = HandleInfoCommand(player, parts);
	}
	else if (cmdName == "refresh") {
		commandResult = HandleRefreshCommand(player, parts);
	}
	else if (cmdName == "list" || cmdName == "help") {
		commandResult = HandleListCommand(player, parts);
	}
	else if (cmdName == "minigame") {
		commandResult = HandleMinigameCommand(player, parts);
	}
	else {
		std::cout << "未知指令: " << cmdName << std::endl;
		return false;
	}

	// If command was successful and game reference is valid, save the game state
	if (commandResult && game) {
		game->SaveGame();
		// std::cout << "已儲存遊戲進度" << std::endl;
	}

	return commandResult;
}

std::vector<std::string> CommandHandler::GetCommandList(bool detailed) {
	std::vector<std::string> result;

	for (const auto& cmd : commands) {
		if (detailed) {
			std::string cmdInfo = "/" + cmd.name + " - " + cmd.description + "\n";
			cmdInfo += "    用法: " + cmd.usage + "\n";
			cmdInfo += "    範例: " + cmd.example;
			result.push_back(cmdInfo);
		}
		else {
			result.push_back("/" + cmd.name + " - " + cmd.description);
		}
	}

	return result;
}

bool CommandHandler::IsValidCommand(const std::string& cmdName) {
	for (const auto& cmd : commands) {
		if (cmd.name == cmdName) {
			return true;
		}
	}
	return false;
}

void CommandHandler::SetGameReference(Game* gamePtr) {
	game = gamePtr;
}

std::vector<std::string> CommandHandler::SplitCommand(const std::string& input) {
	std::vector<std::string> result;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, ' ')) {
		if (!token.empty()) {
			result.push_back(TrimString(token));
		}
	}

	return result;
}

std::string CommandHandler::TrimString(const std::string& str) {
	auto start = std::find_if_not(str.begin(), str.end(), [](char c) { return std::isspace(c); });
	auto end = std::find_if_not(str.rbegin(), str.rend(), [](char c) { return std::isspace(c); }).base();
	return (start < end) ? std::string(start, end) : std::string();
}

// ================================
// 指令處理區塊
// ================================

bool CommandHandler::HandleMoveCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	if (args.empty()) {
		std::cout << "用法: /move <位置>" << std::endl;
		return false;
	}

	try {
		int position;
		if (args[0] == "to" && args.size() > 1) {
			// 透過地名尋找格子
			std::string tileName;
			for (size_t i = 1; i < args.size(); ++i) {
				tileName += args[i];
				if (i < args.size() - 1) {
					tileName += " ";
				}
			}

			// 取得地圖資料
			std::vector<Tile*> tiles = game->gameMap->getMap().first;
			std::vector<std::string> names = game->gameMap->getMap().second;

			auto it = std::find(names.begin(), names.end(), tileName);
			if (it == names.end()) {
				std::cout << "找不到格子: " << tileName << std::endl;
				return false;
			}
			position = static_cast<int>(std::distance(names.begin(), it));
		}
		else {
			position = std::stoi(args[0]);
			if (position < 0 || position >= game->gameMap->getSize()) {
				std::cout << "無效位置: " << position << std::endl;
				return false;
			}
		}

		player->SetPosition(position);
		std::cout << player->GetName() << " 移動到位置 " << position << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cout << "解析位置時發生錯誤: " << e.what() << std::endl;
		return false;
	}
}

bool CommandHandler::HandleGetCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	if (args.empty()) {
		std::cout << "用法: /get <金額> 或 /get <玩家> <金額>" << std::endl;
		return false;
	}

	try {
		if (args.size() == 1) {
			int amount = std::stoi(args[0]);
			if (amount <= 0) {
				std::cout << "金額必須大於 0!" << std::endl;
				return false;
			}
			player->BuyProperty(-amount);
			std::cout << player->GetName() << " 從銀行獲得 $" << amount << std::endl;
			return true;
		}
		else if (args.size() == 2) {
			std::string targetPlayerName = args[0];
			int amount = std::stoi(args[1]);

			if (amount <= 0) {
				std::cout << "金額必須大於 0!" << std::endl;
				return false;
			}

			Player* targetPlayer = nullptr;
			if (targetPlayerName == "bank") {
				targetPlayer = &Monopoly::bank;
			}
			else {
				targetPlayer = FindPlayerByName(targetPlayerName);
			}

			if (!targetPlayer) {
				std::cout << "找不到玩家: " << targetPlayerName << std::endl;
				return false;
			}

			targetPlayer->BuyProperty(-amount);
			std::cout << targetPlayerName << " 從銀行獲得 $" << amount << std::endl;
			return true;
		}
		else {
			std::cout << "用法: /get <金額> 或 /get <玩家> <金額>" << std::endl;
			return false;
		}
	}
	catch (const std::exception& e) {
		std::cout << "解析金額時發生錯誤: " << e.what() << std::endl;
		return false;
	}
}

bool CommandHandler::HandleGiveCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	if (args.size() < 2) {
		std::cout << "用法: /give <玩家> <金額>" << std::endl;
		return false;
	}

	try {
		std::string targetPlayerName = args[0];
		int amount = std::stoi(args[1]);

		if (amount <= 0) {
			std::cout << "金額必須大於 0!" << std::endl;
			return false;
		}

		if (player->GetMoney() < amount) {
			std::cout << "金額不足!" << std::endl;
			return false;
		}

		Player* targetPlayer = nullptr;
		if (targetPlayerName == "bank") {
			targetPlayer = &Monopoly::bank;
		}
		else {
			targetPlayer = FindPlayerByName(targetPlayerName);
		}

		if (!targetPlayer) {
			std::cout << "找不到玩家: " << targetPlayerName << std::endl;
			return false;
		}

		player->Pay(targetPlayer, amount);
		std::cout << player->GetName() << " 給予 " << targetPlayerName << " $" << amount << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cout << "解析金額時發生錯誤: " << e.what() << std::endl;
		return false;
	}
}

bool CommandHandler::HandleCardCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	if (args.empty()) {
		std::cout << "用法: /card <卡牌名稱>" << std::endl;
		std::cout << "可用卡牌: 控制骰子, 火箭卡, 命運卡" << std::endl;
		return false;
	}

	std::string cardName;
	for (size_t i = 0; i < args.size(); ++i) {
		cardName += args[i];
		if (i < args.size() - 1) {
			cardName += " ";
		}
	}
	std::transform(cardName.begin(), cardName.end(), cardName.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (cardName == "control dice" || cardName == "控制骰子" || cardName == "control") {
		player->AddItem(new ControlDiceItem());
		std::cout << player->GetName() << " 獲得卡牌: 控制骰子" << std::endl;
		return true;
	}
	else if (cardName == "rocket card" || cardName == "火箭卡" || cardName == "rocket") {
		player->AddItem(new RocketCard());
		std::cout << player->GetName() << " 獲得卡牌: 火箭卡" << std::endl;
		return true;
	}
	else if (cardName == "fate card" || cardName == "命運卡" || cardName == "fate") {
		player->AddItem(new FateCard());
		std::cout << player->GetName() << " 獲得卡牌: 命運卡" << std::endl;
		return true;
	}
	else {
		std::cout << "未知卡牌: " << cardName << std::endl;
		std::cout << "可用卡牌: 控制骰子, 火箭卡, 命運卡" << std::endl;
		return false;
	}
}

bool CommandHandler::HandleGameStateCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	if (args.empty()) {
		std::cout << "用法: /gamestate <狀態>" << std::endl;
		std::cout << "可用狀態: moved, finish" << std::endl;
		return false;
	}

	std::string stateStr = args[0];

	if (stateStr == "end" || stateStr == "finish" || stateStr == "over") {
		game->gameOver = true;
		std::cout << "遊戲已標記為結束! 下回合將顯示最終結果。" << std::endl;
		return true;
	}
	else if (stateStr == "moved") {
		auto currentTile = game->gameMap->GetTileAt(player->GetPosition());
		currentTile->OnLand(game->getCurrentPlayer());
		return true;
	}
	else {
		std::cout << "未知狀態: " << stateStr << std::endl;
		std::cout << "可用狀態: moved, round_end, finish" << std::endl;
		return false;
	}
}

bool CommandHandler::HandleInfoCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	std::cout << "+------------+--------+-------+------------+--------------------------+\n";
	std::cout << "| Player     | Money  | Pos   | Status     | Items                    |\n";
	std::cout << "+------------+--------+-------+------------+--------------------------+\n";

	const std::string RESET = "\033[0m";

	for (int i = 0; i < game->players.size(); i++) {
		std::string colorCode = Monopoly::GetColorCode(game->players[i]->GetColor());
		std::cout << colorCode;

		std::string status;
		if (game->players[i]->inHospital) {
			status = "住院 (" + std::to_string(game->players[i]->hosipitalDay) + ")";
		}
		else {
			status = "正常";
		}

		std::string itemStr;
		if (game->players[i]->GetItem().empty()) {
			itemStr = "無";
		}
		else {
			for (size_t j = 0; j < game->players[i]->GetItem().size(); j++) {
				itemStr += game->players[i]->GetItem()[j]->GetName();
				if (j != game->players[i]->GetItem().size() - 1)
					itemStr += ", ";
			}
		}

		std::cout << RESET << "| "
			<< colorCode << "[" + std::string(1, '1' + i) + "] "
			<< RESET << std::left << std::setw(6) << game->players[i]->GetName()
			<< " | " << std::right << std::setw(6) << game->players[i]->GetMoney()
			<< " | " << std::right << std::setw(5) << game->players[i]->GetPosition()
			<< " | " << std::left << std::setw(10) << status
			<< " | " << std::left << std::setw(24) << itemStr
			<< " |\n";

		std::cout << "\033[0m";
	}
	std::cout << "+------------+--------+-------+------------+--------------------------+\n";

	return true;
}

bool CommandHandler::HandleRefreshCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	game->Clear();
	std::cout << "地圖已重新繪製。" << std::endl;

	return true;
}

bool CommandHandler::HandleListCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	bool detailed = false;

	if (!args.empty() && args[0] == "-a") {
		detailed = true;
	}

	std::cout << "可用指令:" << std::endl;
	if (detailed) {
		std::cout << "/move - 移動玩家至指定位置（支援格子名稱或數字）" << std::endl;
		std::cout << "    用法: /move <位置>" << std::endl;
		std::cout << "    範例: /move 5、/move to USA" << std::endl;

		std::cout << "/get - 從系統取得金錢（自己或指定玩家）" << std::endl;
		std::cout << "    用法: /get <金額> 或 /get <玩家> <金額>" << std::endl;
		std::cout << "    範例: /get 1000、/get Bob 500" << std::endl;

		std::cout << "/give - 給指定玩家金錢（從自己扣除）" << std::endl;
		std::cout << "    用法: /give <玩家> <金額>" << std::endl;
		std::cout << "    範例: /give Alice 300" << std::endl;

		std::cout << "/card - 取得指定名稱的卡牌" << std::endl;
		std::cout << "    用法: /card <卡牌名稱>" << std::endl;
		std::cout << "    範例: /card Rocket Card" << std::endl;

		std::cout << "/gamestate - 強制改變當前遊戲狀態" << std::endl;
		std::cout << "    用法: /gamestate <狀態>" << std::endl;
		std::cout << "    範例: /gamestate finish" << std::endl;

		std::cout << "/info - 顯示所有玩家資訊" << std::endl;
		std::cout << "    用法: /info" << std::endl;
		std::cout << "    範例: /info" << std::endl;

		std::cout << "/refresh - 強制重新繪製地圖" << std::endl;
		std::cout << "    用法: /refresh" << std::endl;
		std::cout << "    範例: /refresh" << std::endl;

		std::cout << "/list - 顯示所有可用指令（可加 -a 顯示用法）" << std::endl;
		std::cout << "    用法: /list [-a]" << std::endl;
		std::cout << "    範例: /list -a" << std::endl;

		std::cout << "/help - 顯示所有可用指令（可加 -a 顯示用法）" << std::endl;
		std::cout << "    用法: /help [-a]" << std::endl;
		std::cout << "    範例: /help -a" << std::endl;

		std::cout << "/minigame - 直接進入小遊戲選單" << std::endl;
		std::cout << "    用法: /minigame" << std::endl;
		std::cout << "    範例: /minigame" << std::endl;

		std::cout << "\n可用卡牌類型:" << std::endl;
		std::cout << "1. 控制骰子 - 可以指定自己的骰子點數" << std::endl;
		std::cout << "2. 火箭卡 - 讓指定玩家住院兩回合" << std::endl;
		std::cout << "3. 命運卡 - 強制觸發命運事件" << std::endl;

		std::cout << "\n可使用遊戲狀態:" << std::endl;
		std::cout << "1. moved - 觸發停留地塊對應事件" << std::endl;
		std::cout << "2. finish - 遊戲強制結束" << std::endl;

	}
	else {
		std::cout << "/move - 移動玩家至指定位置（支援格子名稱或數字）" << std::endl;
		std::cout << "/get - 從系統取得金錢（自己或指定玩家）" << std::endl;
		std::cout << "/give - 給指定玩家金錢（從自己扣除）" << std::endl;
		std::cout << "/card - 取得指定名稱的卡牌" << std::endl;
		std::cout << "/gamestate - 強制改變當前遊戲狀態" << std::endl;
		std::cout << "/info - 顯示所有玩家資訊" << std::endl;
		std::cout << "/refresh - 強制重新繪製地圖" << std::endl;
		std::cout << "/list - 顯示所有可用指令（可加 -a 顯示用法）" << std::endl;
		std::cout << "/help - 顯示所有可用指令（可加 -a 顯示用法）" << std::endl;
		std::cout << "/minigame - 直接進入小遊戲選單" << std::endl;
	}

	return true;
}

bool CommandHandler::HandleMinigameCommand(std::shared_ptr<Player> player, const std::vector<std::string>& args) {
	if (!game) {
		std::cout << "遊戲尚未初始化!" << std::endl;
		return false;
	}

	std::vector<std::string> gameOptions = {
		"賽馬",
		"射龍門",
		"返回"
	};

	std::cout << "選擇要遊玩的迷你遊戲:" << std::endl;
	for (size_t i = 0; i < gameOptions.size(); ++i) {
		std::cout << (i + 1) << ". " << gameOptions[i] << std::endl;
	}

	int choice = -1;
	while (choice < 0 || choice >= static_cast<int>(gameOptions.size())) {
		std::cout << "輸入選項編號 (1-" << gameOptions.size() << "): ";
		std::string input;
		std::getline(std::cin, input);

		try {
			choice = std::stoi(input) - 1;
		}
		catch (...) {
			choice = -1;
		}
	}

	if (choice == 0) {
		HorseRacing miniGame;
		miniGame.init(player.get());
		miniGame.gameStart();
		return true;
	}
	else if (choice == 1) {
		SheLongMen miniGame;
		miniGame.init(player.get());
		miniGame.gameStart();
		return true;
	}
	else if (choice == 2) {
		std::cout << "返回主選單" << std::endl;
		return false;
	}

	return false;
}
