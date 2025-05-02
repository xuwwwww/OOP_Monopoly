#pragma once
#include <vector>
#include <iostream>
#include "Item.h"
#include "Tile.h"

class Player
{
public:
	/**
	 * 建構子：初始化玩家名稱、顏色與起始金錢
	 */
	Player(std::string name, std::string color, int money);

	/** 是否在醫院中 */
	bool inHospital = false;

	/** 住院天數 */
	int hosipitalDay = 0;

	/**
	 * 取得玩家名稱
	 */
	std::string GetName();

	/**
	 * 取得玩家顏色
	 */
	std::string GetColor();

	/**
	 * 取得目前金錢
	 */
	int GetMoney();

	/**
	 * 取得目前位置
	 */
	int GetPosition();

	/**
	 * 設定玩家位置
	 */
	void SetPosition(int position);

	/**
	 * 取得玩家所持有的道具
	 */
	std::vector<Item*> GetItem();

	/**
	 * 使用道具（尚未實作）
	 */
	void UseItem(Item* item);

	/**
	 * 增加一個道具（尚未實作）
	 */
	void AddItem(Item* item);

	/**
	 * 依擲骰點數移動玩家位置
	 * @param steps 擲骰數
	 * @param mapSize 地圖總格數
	 */
	void Move(int steps, int mapSize);

	/**
	 * 購買地產
	 * @param price 價格
	 * @return 是否購買成功
	 */
	bool BuyProperty(int price);

	/**
	 * 付錢給其他玩家
	 * @param other 接收金錢的玩家
	 * @param amount 金額
	 */
	void Pay(Player* other, int amount);

	/**
	 * 印出玩家目前狀態
	 */
	void PrintStatus();

private:
	/** 玩家名稱 */
	std::string name;

	/** 玩家顏色 */
	std::string color;

	/** 玩家金錢 */
	int money;

	/** 玩家目前位置（地圖格編號） */
	int position;

	/** 持有道具清單 */
	std::vector<Item*> item;
};
