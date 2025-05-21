#pragma once
#include <vector>
#include <iostream>
#include "Item.h"
#include "Tile.h"

class Player
{
public:
	/**
	 * 建構子：隨便塞個名字、顏色跟錢進去
	 */
	Player(std::string name, std::string color, int money);

	/** 是否在醫院中 */
	bool inHospital = false;

	/** 住院天數 */
	int hosipitalDay = 0;

	/**
	 * 拿玩家名字
	 */
	std::string GetName();

	/**
	 * 拿玩家顏色
	 */
	std::string GetColor();

	/**
	 * 看看剩多少錢
	 */
	int GetMoney();

	/**
	 * 看看在哪一格
	 */
	int GetPosition();

	/**
	 * 把玩家丟到某一格
	 */
	void SetPosition(int position);

	/**
	 * 拿到玩家的道具清單
	 */
	std::vector<Item*> GetItem();

	/**
	 * 使用道具（還沒做好）
	 */
	void UseItem(Item* item);

	/**
	 *  將 item 加入 vector
	 */
	void AddItem(Item* item);

	/**
	 * 移動位置：就是 position + steps 再 mod 一下地圖大小
	 */
	void Move(int steps, int mapSize);

	/**
	 * 買道具：有錢就扣一扣回傳 true，沒錢就乖乖回傳 false
	 */
	bool BuyItem(int price);

	/**
	 *  將 property(tile) 加入 vector
	 */
	void AddProperty(Tile* p);

	/**
	 * 買地：有錢就扣一扣回傳 true，沒錢就乖乖回傳 false
	 */
	bool BuyProperty(int price);

	/**
	 * 賣地
	 */
	void SellProperty(Tile* p);

	/**
	 * 回傳 property vector
	 */
	std::vector<Tile*> GetProperty();

	/**
	 * 付錢：把錢轉給別人
	 */
	void Pay(Player* other, int amount);

	/**
	 * 印出玩家資訊：啥名字、位置跟多少錢
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

	/** 擁有的土地 */
	std::vector<Tile*>property;
};
