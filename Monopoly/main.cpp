#include "Game.h"
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

// 一個簡單的選單功能，因為Game物件還未完全初始化
int SimpleUserChoice(const std::string& question, const std::vector<std::string>& options) {
	int selected = 0;

	while (true) {
		system("cls"); // 清除畫面
		std::cout << question << std::endl << std::endl;
		
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

int main() {
	Game game;
	
	// 檢查是否有存檔
	if (game.HasSavedGame()) {
		std::cout << "發現上次未完成的遊戲進度！" << std::endl;
		
		// 提供選項
		std::string question = "是否要載入上次的遊戲進度？";
		std::vector<std::string> options = {
			"載入舊遊戲",
			"開始新遊戲"
		};
		
		// 使用獨立的選單功能，而不是依賴Game中的方法
		int choice = SimpleUserChoice(question, options);
		
		if (choice == 0) {
			// 載入舊遊戲
			if (game.LoadGame()) {
				std::cout << "繼續上次的遊戲！" << std::endl;
				game.StartGame();
			} else {
				std::cout << "載入失敗，將開始新遊戲。" << std::endl;
				game.InitGame();
				game.StartGame();
			}
		} else {
			// 開始新遊戲前刪除舊存檔
			game.DeleteSaveGame();
			game.InitGame();
			game.StartGame();
		}
	} else {
		// 沒有存檔，正常開始遊戲
		game.InitGame();
		game.StartGame();
	}
	
	return 0;
}