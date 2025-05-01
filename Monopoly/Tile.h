#pragma once
#include <iostream>
#include <vector>

class Player;

/**
 * 地圖格類型定義
 */
enum TileType {
	START,
	PROPERTY,
	SHOP,
	HOSPITAL,
	CHANCE,
	FATE,
	GAME,
};

/**
 * 抽象基礎類別：地圖格（Tile）
 */
class Tile
{
public:
	std::string color = "";	// 儲存顏色資訊

	/**
	 * 取得此格的顏色
	 * @return 回傳顏色字串
	 */
	std::string getColor();

	/**
	 * 當玩家抵達此格時觸發的效果（可被覆寫）
	 */
	virtual void OnLand(Player* p);

	/**
	 * 顯示選項選單，供使用者做選擇
	 * @param idx 當前選項索引（可用來顯示預選項）
	 * @param question 問題敘述
	 * @param options 可選項目列表
	 * @return 選擇的索引值
	 */
	int GetUserChoice(int idx, const std::string question, const std::vector<std::string> options);

	/**
	 * 以 ASCII 圖示顯示房屋等級
	 * @param n 房屋等級（屋數）
	 */
	void PrintHouse(int n);
};

/**
 * 起點格（每回合經過會加錢）
 */
class StartTile : public Tile {
public:
	/**
	 * 建構起點格，預設顏色資訊
	 */
	StartTile();

	/**
	 * 玩家抵達起點格時觸發效果
	 */
	void OnLand(Player* p) override;
};

/**
 * 地產格（可購買與收租）
 */
class PropertyTile : public Tile {
private:
	int level;
	int price;
	std::string name;
	Player* owner;

public:
	/**
	 * 建構地產格
	 * @param id 地產等級
	 * @param price 地價
	 * @param name 地名
	 */
	PropertyTile(int level, int price, std::string name);

	/**
	 * 取得地價
	 */
	int GetPrice();

	/**
	 * 升級地產
	 */
	void Upgrade();

	/**
	 * 取得地產擁有者
	 */
	Player* GetOwner();

	/**
	 * 設定地產擁有者
	 */
	void SetOwner(Player* p);

	/**
	 * 玩家抵達此地產格時觸發效果
	 */
	void OnLand(Player* p) override;
};

/**
 * 商店格（可購買道具）
 */
class ShopTile : public Tile {
public:
	/**
	 * 建構起點格，預設顏色資訊
	 */
	ShopTile();

	/**
	 * 玩家抵達商店格時觸發效果
	 */
	void OnLand(Player* p) override;
};

/**
 * 醫院格（住院暫停回合）
 */
class HospitalTile : public Tile {
public:
	/**
	 * 建構起點格，預設顏色資訊
	 */
	HospitalTile();

	/**
	 * 玩家抵達醫院格時觸發效果
	 */
	void OnLand(Player* p) override;
};

/**
 * 機會格（觸發隨機事件）
 */
class ChanceTile : public Tile {
public:
	/**
	 * 建構起點格，預設顏色資訊
	 */
	ChanceTile();

	/**
	 * 玩家抵達機會格時觸發效果
	 */
	void OnLand(Player* p) override;
};

/**
 * 命運格（觸發未知效果）
 */
class FateTile : public Tile {
public:
	/**
	 * 建構起點格，預設顏色資訊
	 */
	FateTile();

	/**
	 * 玩家抵達命運格時觸發效果
	 */
	void OnLand(Player* p) override;
};

/**
 * 小遊戲格（觸發小遊戲）
 */
class MiniGameTile : public Tile {
public:
	/**
	 * 建構起點格，預設顏色資訊
	 */
	MiniGameTile();

	/**
	 * 玩家抵達小遊戲格時觸發效果
	 */
	void OnLand(Player* p) override;
};
