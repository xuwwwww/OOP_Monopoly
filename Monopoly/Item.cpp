#include "Item.h"
#include "Player.h"
#include "Monopoly.h"
#include "Game.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

bool ControlDiceItem::Use(Player* p)
{
	// 控制骰子：讓玩家選擇擲骰子點數的效果
	std::cout << "請選擇要前進的步數 (1-6)：\n";

	int selected = 1;
	bool confirmed = false;

	// 使用方向鍵上下選擇要走的步數
	while (!confirmed) {
		std::cout << "\033[2J\033[H"; // 清空螢幕
		std::cout << "控制骰子：請選擇要前進的步數\n\n";

		// 顯示選項
		for (int i = 1; i <= 6; ++i) {
			if (i == selected) {
				std::cout << " > " << "【" << i << "】" << "\n";
			}
			else {
				std::cout << "   " << "【" << i << "】" << "\n";
			}
		}

		// 接收使用者輸入
		int key = _getch();
		if (key == 224) {
			key = _getch();
			if (key == 72) selected = (selected + 4) % 6 + 1; // 上
			if (key == 80) selected = selected % 6 + 1; // 下
		}
		else if (key == '\r') { // Enter 鍵確認
			confirmed = true;
		}
	}

	std::cout << "你選擇了前進 " << selected << " 步！\n";

	// 取得地圖大小（此處假設為28格，實際上應取自 Game 類別或相關邏輯）
	// 在真實場景中，這個值應該從遊戲邏輯中獲取
	const int mapSize = 28;

	// 移動玩家
	p->Move(selected, mapSize);

	return true;
}

bool RocketCard::Use(Player* p) {
	// 獲取所有玩家列表 (需要訪問Game對象)
	auto game = Monopoly::game;
	if (!game) {
		std::cout << "遊戲未初始化！" << std::endl;
		Monopoly::WaitForEnter();
		return false;
	}

	// 建立玩家選項
	std::vector<std::string> playerOptions;
	std::vector<Player*> availablePlayers;

	// 遍歷所有玩家，排除當前玩家自己
	for (int i = 0; i < game->players.size(); i++) {
		Player* p = game->players[i];

		if (p == game->getCurrentPlayer()) {
			continue;
		}

		if (p->inHospital == true) {
			continue;
		}

		// 添加到選項中
		playerOptions.push_back(p->GetName());
		availablePlayers.push_back(p);
	}

	// 如果沒有其他玩家可選
	if (playerOptions.empty()) {
		std::cout << "沒有可以指定的玩家！" << std::endl;
		Monopoly::WaitForEnter();
		return false;
	}

	playerOptions.push_back("取消");

	// 讓用戶選擇一個玩家
	std::string question = "選擇要送入醫院的玩家（不含已住院玩家）：";
	int choice = Monopoly::GetUserChoice(question, playerOptions, true, false);

	// 如果用戶選擇取消
	if (choice >= static_cast<int>(availablePlayers.size())) {
		std::cout << "取消使用火箭卡。" << std::endl;
		Monopoly::WaitForEnter();
		return false;
	}

	// 將選擇的玩家送入醫院
	Player* targetPlayer = availablePlayers[choice];
	targetPlayer->inHospital = true;
	targetPlayer->hosipitalDay = 0;  // 從0開始計數，需要過兩回合

	std::cout << targetPlayer->GetName() << " 被送入醫院，將住院兩回合！" << std::endl;
	Monopoly::WaitForEnter();

	return true;
}


bool FateCard::Use(Player* p) {
	// 觸發命運事件
	std::cout << "使用命運卡，觸發命運事件！" << std::endl;

	// 隨機決定命運事件
	int eventType = rand() % 5;

	switch (eventType) {
	case 0:
		// 獲得金錢
	{
		int amount = 100 + rand() % 400;  // 100-500的隨機數
		p->BuyProperty(-amount);  // 負值代表獲得錢
		std::cout << "好運降臨！你獲得了 $" << amount << " 的獎勵！" << std::endl;
	}
	break;
	case 1:
		// 失去金錢
	{
		int amount = 50 + rand() % 200;  // 50-250的隨機數
		p->BuyProperty(amount);  // 正值代表失去錢
		std::cout << "不幸降臨！你失去了 $" << amount << "！" << std::endl;
	}
	break;
	case 2:
		// 前進幾步
	{
		int steps = 1 + rand() % 6;  // 1-6的隨機數
		const int mapSize = 28;  // 假設地圖大小
		p->Move(steps, mapSize);
		std::cout << "你被命運之風吹向前方 " << steps << " 步！" << std::endl;
	}
	break;
	case 3:
		// 後退幾步
	{
		int steps = -1 * (1 + rand() % 3);  // -1到-3的隨機數
		const int mapSize = 28;  // 假設地圖大小
		p->Move(steps + mapSize, mapSize);  // 加上地圖大小以處理負數
		std::cout << "你被命運之風吹向後方 " << -steps << " 步！" << std::endl;
	}
	break;
	case 4:
		// 獲得道具
	{
		Item* newItem = new ControlDiceItem();
		p->AddItem(newItem);
		std::cout << "命運給予你一張控制骰子道具卡！" << std::endl;
	}
	break;
	}

	Monopoly::WaitForEnter();

	return true;
}

bool DestroyCard::Use(Player* p) {
	auto game = Monopoly::game;
	if (!game) {
		std::cout << "遊戲未初始化！" << std::endl;
		Monopoly::WaitForEnter();
		return false;
	}

	// 建立玩家選項
	std::vector<std::string> playerOptions;
	std::vector<Player*> availablePlayers;

	// 遍歷所有玩家，排除當前玩家自己
	for (int i = 0; i < game->players.size(); i++) {
		Player* p = game->players[i];

		if (p == game->getCurrentPlayer()) {
			continue;
		}

		// 添加到選項中
		playerOptions.push_back(p->GetName());
		availablePlayers.push_back(p);
	}

	// 如果沒有其他玩家可選
	if (playerOptions.empty()) {
		std::cout << "沒有可以指定的玩家！" << std::endl;
		Monopoly::WaitForEnter();
		return false;
	}

	playerOptions.push_back("取消");

	// 讓用戶選擇一個玩家
	std::string question = "選擇一位玩家，摧毀其一個地產：";
	int choice = Monopoly::GetUserChoice(question, playerOptions, true, false);

	// 如果用戶選擇取消
	if (choice >= static_cast<int>(availablePlayers.size())) {
		std::cout << "取消使用摧毀卡。" << std::endl;
		Monopoly::WaitForEnter();
		return false;
	}

	Player* targetPlayer = availablePlayers[choice];

	// 選擇要摧毀的地產
	std::vector<Tile*> properties = targetPlayer->GetProperty();
	std::vector<std::string> propertyOptions;
	for (Tile* t : properties) {
		propertyOptions.push_back(t->GetName() + " (編號 " + std::to_string(t->number) + ")");
	}
	if (propertyOptions.empty()) {
		std::cout << "沒有可以指定的地產！" << std::endl;
		Monopoly::WaitForEnter();
		return false;
	}

	int propChoice = Monopoly::GetUserChoice("選擇要摧毀的地產：", propertyOptions, true);
	Tile* targetTile = properties[propChoice];

	// 從目標玩家的 property list 移除
	targetPlayer->SellProperty(targetTile, 0);

	std::cout << "你已成功摧毀「" << targetTile->GetName() << "」！\n";

	return true;
}
