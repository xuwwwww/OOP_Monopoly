#include "Player.h"

Player::Player(std::string _name, std::string _color, int _money)
{
	name = _name;
	color = _color;
	money = _money;
	position = 0;
}


std::string Player::GetName()
{
	return name;
}


std::string Player::GetColor()
{
	return color;
}


int Player::GetMoney()
{
	return money;
}


int Player::GetPosition()
{
	return position;
}


void Player::SetPosition(int pos)
{
	this->position = pos;
}


std::vector<Item*> Player::GetItem()
{
	return item;
}

void Player::UseItem(Item* item)
{
}

void Player::AddItem(Item* _item)
{
	item.push_back(_item);
}

void Player::Move(int steps, int mapSize)
{
	position = (position + steps) % mapSize;
}

bool Player::BuyProperty(int price)
{
	if (money < price) {
		std::cout << "財產不足，不得購買\n";
		return false;
	}
	else {
		money -= price;
		return true;
	}
}

void Player::Pay(Player* other, int amount)
{
	// 檢查是否有足夠的錢
	if (money < amount) {
		// 如果是銀行或特殊玩家，允許金額為負
		if (name == "bank") {
			other->money += amount;
			this->money -= amount;
			return;
		}
		
		// 如果沒有足夠的錢，只支付剩餘的金額
		std::cout << name << " 資金不足，只能支付 " << money << " 元。\n";
		other->money += money;
		this->money = 0;
		
		// 檢查是否破產
		std::cout << name << " 已破產！\n";
	}
	else {
		// 有足夠的錢，正常支付
		other->money += amount;
		this->money -= amount;
	}
}

void Player::PrintStatus()
{
	std::cout << "玩家: " << name << " | ";
	std::cout << "位置: " << position << " | ";
	std::cout << "金錢: $" << money << std::endl;
}
