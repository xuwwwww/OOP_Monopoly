#pragma once

#include <iostream>

#include "Tile.h"
#include "Player.h"

class Map
{
public:
	/**
	 * 建構函式：初始化地圖
	 */
	Map();

	/**
	 * 從檔案載入地圖配置（未實作或保留未來擴充）
	 * @param path 檔案路徑
	 */
	void LoadMap(std::string path);

	/**
	 * 輸出整張地圖的狀態，包含玩家位置
	 * @param players 玩家列表
	 */
	void PrintMap(std::vector<Player*>& players);

	/**
	* 取得遊戲地圖的資訊
	* @return 返回包含兩個向量的 `std::pair`：
	*         - 第一個向量是包含地圖格子（`Tile*`）的指標，代表遊戲中的各種格子（例如起點、地產、商店等）
	*         - 第二個向量是包含對應格子名稱的字串（`std::string`），每個格子都有對應的名稱或標識
	*/
	std::pair<std::vector<Tile*>, std::vector<std::string>> getMap();

	/**
	 * 將地圖格加入地圖
	 * @param tile 地圖格實體（繼承自 Tile）
	 * @param name 地名（用於顯示）
	 */
	void addTile(Tile* tile, std::string name);

	/**
	 * 取得指定位置的地圖格物件
	 * @param pos 座標位置（整數）
	 * @return 指定位置上的 Tile*
	 */
	Tile* GetTileAt(int pos);

	/**
	 * 取得地圖大小（格子總數）
	 */
	int getSize();

private:
	std::vector<Tile*> map;               // 地圖格陣列
	std::vector<std::string> names;       // 對應每格的地名（顯示用）

	/**
	 * 初始化預設地圖配置
	 */
	void InitMap();
};

