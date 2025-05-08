#pragma once

#include <iostream>

#include "Tile.h"
#include "Player.h"

class Map
{
public:
	/**
	 * 建構函式：建好一張地圖
	 */
	Map();

	/**
	 * 從檔案載入地圖配置（其實用不到啦）
	 * @param path 檔案路徑
	 */
	void LoadMap(std::string path);

	/**
	 * 印出地圖，順便標注玩家位置
	 * @param players 玩家列表
	 */
	void PrintMap(std::vector<Player*>& players);

	/**
	* 取得整個地圖的資訊（格子跟名稱）
	* @return 格子指標跟地名的配對
	*/
	std::pair<std::vector<Tile*>, std::vector<std::string>> getMap();

	/**
	 * 加格子進地圖，連名字一起塞
	 * @param tile 格子指標
	 * @param name 地名
	 */
	void addTile(Tile* tile, std::string name);

	/**
	 * 找出某位置的格子
	 * @param pos 位置
	 * @return 那格的指標
	 */
	Tile* GetTileAt(int pos);

	/**
	 * 看地圖有幾格
	 */
	int getSize();

private:
	std::vector<Tile*> map;               // 地圖格陣列
	std::vector<std::string> names;       // 對應每格的地名（顯示用）

	/**
	 * 建出一張預設地圖
	 */
	void InitMap();
};

