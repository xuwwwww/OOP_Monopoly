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
	int number = 0;

	/**
	 * return name
	 */
	virtual std::string GetName();

	/**
	 * 拿這格的顏色
	 */
	std::string getColor();

	/**
	 * 玩家踩到這格會觸發的事件（子類實作具體內容）
	 */
	virtual void OnLand(Player* p);
};

/**
 * 起點格（每回合經過會加錢）
 */
class StartTile : public Tile {
private:
	std::string name = "start";
public:
	/**
	 * 建構起點格，預設顏色資訊
	 */
	StartTile(int);

	/**
	 * 取得名稱
	 */
	std::string GetName() override;

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
	PropertyTile(int level, int price, std::string name, int n);

	/**
	 * 取得名稱
	 */
	std::string GetName() override;

	/**
	 * 取得地價
	 */
	int GetPrice();

	/**
	 * 取得地產等級
	 * @return 地產等級
	 */
	int GetLevel();

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
	ShopTile(int n);

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
	HospitalTile(int n);

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
	ChanceTile(int n);

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
	FateTile(int n);

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
	MiniGameTile(int n);

	/**
	 * 玩家抵達小遊戲格時觸發效果
	 */
	void OnLand(Player* p) override;
};
