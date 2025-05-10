#pragma once

#include "Player.h"
#include "Map.h"
#include <fstream>
#include <string>

enum class State { INIT, START, MOVED, ROUND_END, FINISH };

class Game
{
public:
	/**
	 * 建構子，初始化遊戲狀態（玩家索引、遊戲是否結束）
	 */
	Game();

	/**
	 * 初始化地圖與玩家資訊，並選擇人數與名稱
	 */
	void InitGame();

	/**
	 * 啟動遊戲主流程，每回合執行直到勝利條件達成
	 */
	void StartGame();

	/**
	 * 執行單回合流程（擲骰、移動、觸發格子事件、換人）
	 */
	void NextTurn();

	/**
	 * 判斷是否有玩家達成勝利條件（如資金達標）
	 * @return 若有玩家獲勝則回傳 true
	 */
	bool CheckWinCondition();

	/**
	 * 遊戲結束時顯示所有玩家最終資訊
	 */
	void EndGame();


	void HandlePlayerAction(Player* p);

	/**
	 * 清除畫面，並重新印出地圖與玩家資訊
	 */
	void Clear();

	/**
	 * 印出目前地圖狀態（格子名稱、擁有者等）
	 */
	void PrintMapStatus();

	/**
	 * 印出所有玩家的狀態（位置、金錢、道具等）
	 */
	void PrintPlayerStatus();

	/**
	 * 保存遊戲進度到檔案
	 * @param filename 保存檔案的路徑
	 * @return 是否成功保存
	 */
	bool SaveGame(const std::string& filename = "save.json");

	/**
	 * 從檔案載入遊戲進度
	 * @param filename 載入檔案的路徑
	 * @return 是否成功載入
	 */
	bool LoadGame(const std::string& filename = "save.json");

	/**
	 * 檢查是否有保存的遊戲進度
	 * @param filename 保存檔案的路徑
	 * @return 是否有保存的進度
	 */
	bool HasSavedGame(const std::string& filename = "save.json");

	/**
	 * 刪除保存的遊戲進度
	 * @param filename 保存檔案的路徑
	 * @return 是否成功刪除
	 */
	bool DeleteSaveGame(const std::string& filename = "save.json");

	/**
	 * 處理玩家破產的情況
	 * @param bankruptPlayer 破產的玩家
	 * @param creditor 債權玩家（導致破產的玩家）
	 */
	void HandlePlayerBankruptcy(Player* bankruptPlayer, Player* creditor);

	Player* getCurrentPlayer();

	Map* gameMap;					// 地圖物件
private:
	std::vector<Player*>players;	// 所有玩家指標
	int currentPlayerIdx;			// 目前回合的玩家索引
	bool gameOver;					// 是否結束遊戲

	/**
	 * 輔助函式：印出兩顆骰子的 ASCII 圖案
	 */
	void PrintDice(int, int);

	/**
	 * 執行雙骰動畫並回傳總點數
	 */
	int RollDiceWithAsciiAnimation();

	/**
	* 根據顏色名稱取得對應的顏色碼（文字顏色）
	* @param colorName 顏色名稱（如 "red", "green", "blue" 等）
	* @return 對應的顏色碼
	*/
	std::string GetColorCode(std::string colorName);

	/**
	* 根據顏色名稱取得對應的顏色碼（背景顏色）
	* @param colorName 背景顏色名稱（如 "red", "green", "yellow" 等）
	* @return 對應的背景顏色碼
	*/
	std::string GetBackgroundColorCode(std::string colorName);
};

